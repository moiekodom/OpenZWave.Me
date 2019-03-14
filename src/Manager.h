#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include "Defs.h"
#include "Driver.h"

namespace OpenZWaveMe
{
    class Manager
    {
        friend class Driver;

        //////////////////////////////////////////////////

        private:
            static Manager* s_instance;
            list<Driver*> m_pendingDrivers;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            Manager();
            virtual ~Manager();

        public:
            static Manager* Create();
            static void Destroy();

            static Manager* Get() {return s_instance;}
            bool AddDriver(string const& _controllerPath, ControllerInterface const& _interface = OpenZWaveMe::Driver::ControllerInterface_ZWaySerial);
    };
}

#endif // MANAGER_H_INCLUDED
