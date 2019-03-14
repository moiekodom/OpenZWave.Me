#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include <Driver.h>

#include <platform/TimeStamp.h>

#include "Defs.h"

#include "platform/ZWaySerialController.h"

using OpenZWave::TimeStamp;

namespace OpenZWaveMe
{
    class Driver
    {
        friend class Manager;

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
            string m_controllerPath;
            ControllerInterface m_controllerInterfaceType;
            Controller* m_controller;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            Driver(string const& _controllerPath, ControllerInterface const& _interface);
            virtual ~Driver();

            bool Init(uint32 _attempts);
            void Start();
            string GetControllerPath() const {return m_controllerPath;}
    };
}

using ControllerInterface = OpenZWaveMe::Driver::ControllerInterface;

#endif // DRIVER_H_INCLUDED
