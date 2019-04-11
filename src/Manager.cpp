#include "Manager.h"

namespace OpenZWaveMe
{
    Manager* Manager::s_instance = NULL;

    Manager::Manager()
    {
        s_instance = this;
    }
    //----------------------------------------------------------------------------------------------------

    Manager::~Manager()
    {
        for_each(m_pendingDrivers.begin(), m_pendingDrivers.end(), [](Driver* _driver) {delete _driver;});

        for_each(m_readyDrivers.begin(), m_readyDrivers.end(), [](pair<uint32, Driver*> _keyValue) {delete _keyValue.second;});
    }
    //----------------------------------------------------------------------------------------------------

    Manager* Manager::Create()
    {
        if (Options::Get() && Options::Get()->AreLocked())
        {
            if (NULL == s_instance)
            {
                s_instance = new Manager();
            }

            return s_instance;
        }

        //OZW_FATAL_ERROR();

        return NULL;
    }
    //----------------------------------------------------------------------------------------------------

    void Manager::Destroy()
    {
        delete s_instance;

        s_instance = NULL;
    }
    //----------------------------------------------------------------------------------------------------

    void Manager::SetDriverReady(Driver* _driver, bool _success)
    {
    }
    //----------------------------------------------------------------------------------------------------

    bool Manager::AddDriver(string const& _controllerPath, Driver::ControllerInterface _interface)
    {
        if (any_of(m_pendingDrivers.begin(), m_pendingDrivers.end(), [&](Driver* _driver) {return _driver->GetControllerPath() == _controllerPath;}))
        {
            Log::Write(OpenZWave::LogLevel_Info, "mgr,     Cannot add driver for controller %s - driver already exists", _controllerPath.c_str());

            return FALSE;
        }

        if (any_of(m_readyDrivers.begin(), m_readyDrivers.end(), [&](pair<uint32, Driver*> _keyValue) {return _keyValue.second->GetControllerPath() == _controllerPath;}))
        {
            Log::Write(OpenZWave::LogLevel_Info, "mgr,     Cannot add driver for controller %s - driver already exists", _controllerPath.c_str());

            return FALSE;
        }

        Driver* driver = new Driver(_controllerPath, _interface);

        m_pendingDrivers.push_back(driver);

        driver->Start();

        Log::Write(OpenZWave::LogLevel_Info, "mgr,     Added driver for controller %s", _controllerPath.c_str());

        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    bool Manager::RemoveDriver(string const& _controllerPath)
    {
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    bool Manager::AddWatcher(pfnOnNotification_t _watcher, void* _context)
    {
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------
}
