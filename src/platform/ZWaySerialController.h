#ifndef SERIALCONTROLLER_H_INCLUDED
#define SERIALCONTROLLER_H_INCLUDED

#include <ZWayLib.h>

#include "../Defs.h"

#include "Controller.h"

using OpenZWaveMe::Controller;

namespace OpenZWaveMe
{
    class ZWaySerialController : public Controller
    {
        private:
            ZWError m_lastError;
            ZWay m_zway;

        public:
            ZWaySerialController();
            virtual ~ZWaySerialController();

            bool Open(string const& _serialControllerName);
            bool Close();
            uint32 Write(uint8* _buffer, uint32 _length);
    };
}

#endif // SERIALCONTROLLER_H_INCLUDED
