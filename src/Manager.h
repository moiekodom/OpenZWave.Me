#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <Options.h>

#include "Defs.h"

#include "Notification.h"
#include "Driver.h"

using OpenZWave::Options;

using OpenZWaveMe::Notification;
using OpenZWaveMe::Driver;

namespace OpenZWaveMe
{
    class Manager
    {
        friend class Driver;

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public:
            using pfnOnNotification_t = void (*)(Notification* _pNotification, void* _context);

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        private:
            static Manager* s_instance;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            list<Driver*> m_pendingDrivers;
            map<uint32, Driver*> m_readyDrivers;

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            Manager();
            virtual ~Manager();

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            void SetDriverReady(Driver* _driver, bool _success);

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        public:
            static Manager* Create();
            static void Destroy();
            static Manager* Get() {return s_instance;}

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            bool AddDriver(string const& _controllerPath, Driver::ControllerInterface _interface = Driver::ControllerInterface_ZWaySerial);
            bool RemoveDriver(string const& _controllerPath);
            bool AddWatcher(pfnOnNotification_t _watcher, void* _context);
    };
}

#endif // MANAGER_H_INCLUDED
