#ifndef ZWAYSERIALCONTROLLER_H_INCLUDED
#define ZWAYSERIALCONTROLLER_H_INCLUDED

// ���������� ������ �������� typedef ��� __socklen_t, ����� ��� ���������� ��� Windows (MinGW) ������: "��� �� ��������".
#include "../Defs.h"

// ����� ���������� <bits/socket.h>, � ������� ���� "typedef __socklen_t socklen_t".
#include <ZWayLib.h>

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

#endif // ZWAYSERIALCONTROLLER_H_INCLUDED
