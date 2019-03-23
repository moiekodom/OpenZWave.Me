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
            static Manager const* Create();
            static void Destroy();
            inline static Manager* Get();

            ////////////////////////////////////////////////////////////////////////////////////////////////////

            bool AddDriver(string const& _controllerPath, Driver::ControllerInterface _interface = Driver::ControllerInterface_ZWaySerial);
            bool RemoveDriver(string const& _controllerPath);
            bool AddWatcher(pfnOnNotification_t _watcher, void* _context);
    };

    inline Manager* Manager::Get() {return s_instance;}
}

#endif // MANAGER_H_INCLUDED
