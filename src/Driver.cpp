#include "Manager.h"
#include "Driver.h"

namespace OpenZWaveMe
{
    Driver::Driver(string const& _controllerPath, ControllerInterface const& _interface) :
        m_waitingForAck(FALSE),
        m_exit(FALSE),
        m_Controller_nodeId(0),
        m_expectedCallbackId(0),
        m_expectedReply(0),
        m_homeId(0),
        m_controllerPath(_controllerPath),
        m_controllerInterfaceType(_interface),
        m_currentControllerCommand(NULL),
        m_controller(NULL),
        m_notificationsEvent(new Event()),
        m_initMutex(new Mutex()),
        m_driverThread(new Thread("driver")),
        m_pollThread(new Thread("poll")),
        m_currentMsg(NULL)
    {
        TimeStamp m_startTime;

        m_controller = new ZWaySerialController();
    }
    //----------------------------------------------------------------------------------------------------

    Driver::~Driver()
    {
        m_controller->Close();
        m_controller->Release();
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::DriverThreadEntryPoint(Event* _exitEvent, void* _context)
    {
        Driver* driver = (Driver*)_context;

        if (driver)
        {
            driver->DriverThreadProc(_exitEvent);
        }
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::PollThreadEntryPoint(Event* _exitEvent, void* _context)
    {
        Driver* driver = (Driver*)_context;

        if (driver)
        {
            driver->PollThreadProc(_exitEvent);
        }
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::DriverThreadProc(Event* _exitEvent)
    {
        uint16 retryTimeout = RETRY_TIMEOUT;
        uint32 attempts     = 0;
        uint32 count        = 11;
        int32 timeout       = Wait::Timeout_Infinite;
        uint32 maxAttempts  = 0;
        TimeStamp retryTimeStamp;
        Wait* waitObjects[11];

        //Options::Get()->GetOptionAsInt("RetryTimeout", &retryTimeout);
        //Options::Get()->GetOptionAsInt("DriverMaxAttempts", (int32*)&maxAttempts);

        while (TRUE)
        {
            if (Init(attempts))
            {
                waitObjects[0]  = _exitEvent;				                            // Thread must exit.
                waitObjects[1]  = m_notificationsEvent;			                        // Notifications waiting to be sent.
                waitObjects[2]  = m_controller;				                            // Controller has received data.
                waitObjects[3]  = m_queueEvent[OpenZWave::Driver::MsgQueue_Command];	// A controller command is in progress.
                waitObjects[4]  = m_queueEvent[OpenZWave::Driver::MsgQueue_Security];	// Security Related Commands (As they have a timeout)
                waitObjects[5]  = m_queueEvent[OpenZWave::Driver::MsgQueue_NoOp];		// Send device probes and diagnostics messages
                waitObjects[6]  = m_queueEvent[OpenZWave::Driver::MsgQueue_Controller];	// A multi-part controller command is in progress
                waitObjects[7]  = m_queueEvent[OpenZWave::Driver::MsgQueue_WakeUp];		// A node has woken. Pending messages should be sent.
                waitObjects[8]  = m_queueEvent[OpenZWave::Driver::MsgQueue_Send];		// Ordinary requests to be sent.
                waitObjects[9]  = m_queueEvent[OpenZWave::Driver::MsgQueue_Query];		// Node queries are pending.
                waitObjects[10] = m_queueEvent[OpenZWave::Driver::MsgQueue_Poll];		// Poll request is waiting.

                while (TRUE)
                {
                    Log::Write(OpenZWave::LogLevel_StreamDetail, "      Top of DriverThreadProc loop." );

                    // If we're waiting for a message to complete, we can only
                    // handle incoming data, notifications and exit events.
                    if (m_waitingForAck || m_expectedCallbackId || m_expectedReply)
                    {
                        count = 3;

                        timeout = m_waitingForAck ? ACK_TIMEOUT : retryTimeStamp.TimeRemaining();

                        if (timeout < 0)
                        {
                            timeout = 0;
                        }
                    }
                    else if (m_currentControllerCommand != NULL)
                    {
                        count = 7;
                    }
                    else
                    {
                        Log::QueueClear(); // clear the log queue when starting a new message
                    }

                    // Wait for something to do
                    int32 res = Wait::Multiple(waitObjects, count, timeout);

                    switch (res)
                    {
                        case -1:

                            // Wait has timed out - time to resend
                            if (m_currentMsg != NULL)
                            {
                                Notification* notification = new Notification(OpenZWave::Notification::Type_Notification);

                                notification->SetHomeAndNodeIds(m_homeId, m_currentMsg->GetTargetNodeId());
                                notification->SetNotification(OpenZWave::Notification::Code_Timeout);

                                QueueNotification(notification);
                            }

                            if (WriteMsg("Wait Timeout"))
                            {
                                retryTimeStamp.SetTime(retryTimeout);
                            }

                            break;
                        case 0:
                        {

                            // Exit has been signalled
                            return;
                        }
                        case 1:

                            // Notifications are waiting to be sent
                            NotifyWatchers();

                            break;
                        case 2:

                            // Data has been received
                            ReadMsg();

                            break;
                        default:

                            // All the other events are sending message queue items
                            if (WriteNextMsg((MsgQueue)(res - 3)))
                            {
                                retryTimeStamp.SetTime(retryTimeout);
                            }

                            break;
                    }
                }
            }

            ++attempts;

            if (maxAttempts && attempts >= maxAttempts)
            {
                Manager::Get()->Manager::SetDriverReady(this, FALSE);

                NotifyWatchers();

                break;
            }

            if (attempts < 25)
            {
                // Retry every 5 seconds for the first two minutes
                if (Wait::Single(_exitEvent, 5000) == 0)
                {
                    // Exit signalled.
                    return;
                }
            }
            else
            {
                // Retry every 30 seconds after that
                if (Wait::Single(_exitEvent, 30000) == 0)
                {
                    // Exit signalled.
                    return;
                }
            }
        }
    }
    //----------------------------------------------------------------------------------------------------

    void PollThreadProc(Event* _exitEvent)
    {
    }
    //----------------------------------------------------------------------------------------------------

    bool Driver::Init(uint32 _attempts)
    {
        uint8 nak = NAK;

        m_initMutex->Lock();

        if (m_exit)
        {
            m_initMutex->Unlock();

            return FALSE;
        }

        m_Controller_nodeId = -1;
        m_waitingForAck     = FALSE;

        // Open the controller
        Log::Write(OpenZWave::LogLevel_Info, "  Opening controller %s", m_controllerPath.c_str());

        if (!m_controller->Open(m_controllerPath))
        {
            Log::Write(OpenZWave::LogLevel_Warning, "WARNING: Failed to init the controller (attempt %d)", _attempts );

            m_initMutex->Unlock();

            return FALSE;
        }

        // Controller opened successfully, so we need to start all the worker threads
        m_pollThread->Start(Driver::PollThreadEntryPoint, this);

        // Send a NAK to the ZWave device
        m_controller->Write(&nak, 1);

        // Get/set ZWave controller information in its preferred initialization order
        m_controller->PlayInitSequence(this);

        //If we ever want promiscuous mode uncomment this code.
        //Msg* msg = new Msg( "FUNC_ID_ZW_SET_PROMISCUOUS_MODE", 0xff, REQUEST, FUNC_ID_ZW_SET_PROMISCUOUS_MODE, false, false );
        //msg->Append( 0xff );
        //SendMsg( msg );

        m_initMutex->Unlock();

        // Init successful
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::Start()
    {
        // Start the thread that will handle communications with Z-Wave network
        m_driverThread->Start(Driver::DriverThreadEntryPoint, this);
    }
    //----------------------------------------------------------------------------------------------------

    bool Driver::ReadMsg()
    {
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    bool Driver::WriteMsg(string const& str)
    {
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    bool Driver::WriteNextMsg(MsgQueue _queue)
    {
        return FALSE;
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::QueueNotification(Notification* _notification)
    {
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::NotifyWatchers()
    {
    }
    //----------------------------------------------------------------------------------------------------

    void Driver::SendMsg(Msg* _msg, MsgQueue _queue)
    {
    }
    //----------------------------------------------------------------------------------------------------
}
