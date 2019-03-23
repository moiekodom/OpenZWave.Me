#ifndef VALUEID_H_INCLUDED
#define VALUEID_H_INCLUDED

#include <value_classes/ValueID.h>

#include "../Defs.h"

namespace OpenZWaveMe
{
    class ValueID
    {
        friend class Notification;

        private:
            uint32 m_id;
            uint32 m_idl;
            uint32 m_homeId;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            ValueID() :
                m_id(0),
                m_idl(0),
                m_homeId(0) {}
            ValueID(uint32 _homeId, uint8 _nodeId) :
                m_idl(0),
                m_homeId(_homeId) {m_id = ((uint32)_nodeId) << 24;}
    };
}

#endif // VALUEID_H_INCLUDED
