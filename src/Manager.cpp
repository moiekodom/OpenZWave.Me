#include "Manager.h"

namespace OpenZWaveMe
{
    Manager::Manager()
    {
        s_instance = this;
    }
    //----------------------------------------------------------------------------------------------------

    Manager::~Manager()
    {
    }
    //----------------------------------------------------------------------------------------------------

    Manager* Manager::Create()
    {
        if (NULL == s_instance)
        {
            s_instance = new Manager();
        }

        return s_instance;

        return NULL;
    }
    //----------------------------------------------------------------------------------------------------

    void Manager::Destroy()
    {
        delete s_instance;

        s_instance = NULL;
    }
    //----------------------------------------------------------------------------------------------------

    bool Manager::AddDriver(string const& _controllerPath, ControllerInterface const& _interface)
    {
        for (list<Driver*>::iterator pit = m_pendingDrivers.begin(); pit != m_pendingDrivers.end(); ++pit)
        {
            if (_controllerPath == (*pit)->GetControllerPath())
            {
                Log::Write(OpenZWave::LogLevel_Info, "mgr,     Cannot add driver for controller %s - driver already exists", _controllerPath.c_str());

                return FALSE;
            }
        }

        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------
}
