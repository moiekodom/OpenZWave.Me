#ifndef NOTIFICATION_H_INCLUDED
#define NOTIFICATION_H_INCLUDED

#include <Notification.h>

#include "Defs.h"

#include "value_classes/ValueID.h"

using OpenZWaveMe::ValueID;

namespace OpenZWaveMe
{
    class Notification
    {
        friend class Driver;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        using NotificationType = OpenZWave::Notification::NotificationType;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private:
            uint8 m_byte;
            NotificationType m_type;
            ValueID m_valueId;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            Notification(NotificationType _type)
            {
            }
            ~Notification() {}

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            inline void SetHomeAndNodeIds(uint32 _homeId, uint8 _nodeId);
            void SetNotification(uint8 _noteId) {ASSERT(m_type == NotificationType::Type_Notification || m_type == NotificationType::Type_ControllerCommand); m_byte = _noteId;}
    };

    inline void Notification::SetHomeAndNodeIds(uint32 _homeId, uint8 _nodeId) {m_valueId = ValueID(_homeId, _nodeId);}
}

#endif // NOTIFICATION_H_INCLUDED
