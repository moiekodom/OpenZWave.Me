#include "Driver.h"

namespace OpenZWaveMe
{
    /*static char const* c_controllerCommandNames[] =
    {
        "None",
        "Add Device",
        "Create New Primary",
        "Receive Configuration",
        "Remove Device",
        "Remove Failed Node",
        "Has Node Failed",
        "Replace Failed Node",
        "Transfer Primary Role",
        "Request Network Update",
        "Request Node Neighbor Update",
        "Assign Return Route",
        "Delete All Return Routes",
        "Send Node Information",
        "Replication Send",
        "Create Button",
        "Delete Button"
    }; */

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Driver::Driver(string const& _controllerPath, ControllerInterface const& _interface) :
        m_controllerPath(_controllerPath),
        m_controllerInterfaceType(_interface)
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

    bool Driver::Init(uint32 _attempts)
    {
        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------
}
