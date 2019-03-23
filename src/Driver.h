#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include <platform/TimeStamp.h>
#include <platform/Event.h>
#include <platform/Mutex.h>
#include <platform/Thread.h>
#include <platform/Wait.h>

#include <Msg.h>
#include <Node.h>
#include <Driver.h>

#include "Defs.h"

#include "platform/ZWaySerialController.h"

#include "Notification.h"

using OpenZWave::Msg;
using OpenZWave::Node;
using OpenZWave::TimeStamp;
using OpenZWave::Event;
using OpenZWave::Mutex;
using OpenZWave::Thread;
using OpenZWave::Wait;

using OpenZWaveMe::Controller;
using OpenZWaveMe::Notification;

namespace OpenZWaveMe
{
    class Driver
    {
        friend class Manager;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        using ControllerState   = OpenZWave::Driver::ControllerState;
        using ControllerCommand = OpenZWave::Driver::ControllerCommand;
        using ControllerError   = OpenZWave::Driver::ControllerError;
        using MsgQueue          = OpenZWave::Driver::MsgQueue;

        using pfnControllerCallback_t = void (*)(ControllerState _state, ControllerError _err, void* context);

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public:
            enum ControllerInterface
            {
                ControllerInterface_Unknown = 0,
                ControllerInterface_Serial,
                ControllerInterface_ZWaySerial,
                ControllerInterface_Hid
            };

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private:
            enum MsgQueueCmd
            {
                MsgQueueCmd_SendMsg = 0,
                MsgQueueCmd_QueryStageComplete,
                MsgQueueCmd_Controller
            };

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            struct ControllerCommandItem
            {
                ControllerState		    m_controllerState;
                bool					m_controllerStateChanged;
                bool					m_controllerCommandDone;
                ControllerCommand		m_controllerCommand;
                pfnControllerCallback_t m_controllerCallback;
                ControllerError			m_controllerReturnError;
                void*					m_controllerCallbackContext;
                bool					m_highPower;
                bool					m_controllerAdded;
                uint8					m_controllerCommandNode;
                uint8					m_controllerCommandArg;
                uint8					m_controllerDeviceProtocolInfo[254];
                uint8 					m_controllerDeviceProtocolInfoLength;
            };

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            class MsgQueueItem
            {
            public:
                bool				    m_retry;
                uint8				    m_nodeId;
                Node::QueryStage        m_queryStage;
                MsgQueueCmd			    m_command;
                Msg*				    m_msg;
                ControllerCommandItem*  m_cci;

                ////////////////////////////////////////////////////////////////////////////////////////////////////

                MsgQueueItem() :
                    m_msg(NULL),
                    m_nodeId(0),
                    m_queryStage(Node::QueryStage_None),
                    m_retry(FALSE),
                    m_cci(NULL) {}

                ////////////////////////////////////////////////////////////////////////////////////////////////////

                bool operator == (MsgQueueItem const& _other) const
                {
                    if (_other.m_command == m_command)
                    {
                        if (m_command == MsgQueueCmd_SendMsg)
                        {
                            return ((*_other.m_msg) == (*m_msg));
                        }
                        else if (m_command == MsgQueueCmd_QueryStageComplete)
                        {
                            return ((_other.m_nodeId == m_nodeId) && (_other.m_queryStage == m_queryStage));
                        }
                        else if (m_command == MsgQueueCmd_Controller)
                        {
                            return ((_other.m_cci->m_controllerCommand == m_cci->m_controllerCommand) && (_other.m_cci->m_controllerCallback == m_cci->m_controllerCallback));
                        }
                    }

                    return false;
                }
            };

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            bool m_waitingForAck;
            bool m_exit;
            uint8 m_Controller_nodeId;
            uint8 m_expectedCallbackId;
            uint8 m_expectedReply;
            uint32 m_homeId;
            string m_controllerPath;
            ControllerInterface m_controllerInterfaceType;
            ControllerCommandItem* m_currentControllerCommand;
            Controller* m_controller;
            Event* m_notificationsEvent;
            Event* m_queueEvent[OpenZWave::Driver::MsgQueue_Count];
            Mutex* m_initMutex;
            Thread* m_driverThread;
            Thread* m_pollThread;
            Msg* m_currentMsg;

            list<Notification*> m_notifications;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            Driver(string const& _controllerPath, ControllerInterface const& _interface);
            virtual ~Driver();

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            static void DriverThreadEntryPoint(Event* _exitEvent, void* _context);
            static void PollThreadEntryPoint(Event* _exitEvent, void* _context);

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            void DriverThreadProc(Event* _exitEvent);
            void PollThreadProc(Event* _exitEvent);
            bool Init(uint32 _attempts);
            void Start();
            bool ReadMsg();
            bool WriteMsg(string const& str);
            bool WriteNextMsg(MsgQueue _queue);
            void QueueNotification(Notification* _notification);
            void NotifyWatchers();
            inline string const& GetControllerPath();

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public:
            void SendMsg(Msg* _msg, MsgQueue _queue);
    };

    inline string const& Driver::GetControllerPath() {return m_controllerPath;}
}

#endif // DRIVER_H_INCLUDED
