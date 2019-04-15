#include "ZWaySerialController.h"

namespace OpenZWaveMe
{
    ZWaySerialController::ZWaySerialController() :
        m_lastError(NoError)
    {
        memset(&m_zway, 0, sizeof(ZWay));
    }
    //----------------------------------------------------------------------------------------------------

    ZWaySerialController::~ZWaySerialController()
    {
    }
    //----------------------------------------------------------------------------------------------------

    bool ZWaySerialController::Open(string const& _serialControllerName)
    {
        m_lastError = zway_init(&m_zway, _serialControllerName.c_str(), ZSTR("config"), ZSTR("translations"), ZSTR("ZDDX"), NULL, NULL);

        if (m_lastError != NoError)
        {
            OZW_FATAL_ERROR(m_lastError, "Failed to init ZWay");

            return FALSE;
        }

        return TRUE;
    }
    //----------------------------------------------------------------------------------------------------

    bool ZWaySerialController::Close()
    {
        m_lastError = zway_stop(m_zway);

        zway_terminate(&m_zway);

        return !(bool)m_lastError;
    }
    //----------------------------------------------------------------------------------------------------

    uint32 ZWaySerialController::Write(uint8* _buffer, uint32 _length)
    {
        return 0;
    }
    //----------------------------------------------------------------------------------------------------
}
