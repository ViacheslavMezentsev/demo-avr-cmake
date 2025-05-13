<<<<<<< HEAD
#include "Mutex.h"

#include <string.h>
//#include "error.h"

namespace rtos {

Mutex::Mutex() {
#ifdef CMSIS_OS_RTX
    memset(_mutex_data, 0, sizeof(_mutex_data));
    _osMutexDef.mutex = _mutex_data;
#endif
    _osMutexId = osMutexCreate(&_osMutexDef);
    if (_osMutexId == NULL) {
//        error("Error initializing the mutex object\n");
    }
}

osStatus Mutex::lock(uint32_t millisec) {
    return osMutexWait(_osMutexId, millisec);
}

bool Mutex::trylock() {
    return (osMutexWait(_osMutexId, 0) == osOK);
}

osStatus Mutex::unlock() {
    return osMutexRelease(_osMutexId);
}

}
=======
#include "Mutex.h"

#include <string.h>
//#include "error.h"

namespace rtos {

Mutex::Mutex() {
#ifdef CMSIS_OS_RTX
    memset(_mutex_data, 0, sizeof(_mutex_data));
    _osMutexDef.mutex = _mutex_data;
#endif
    _osMutexId = osMutexCreate(&_osMutexDef);
    if (_osMutexId == NULL) {
//        error("Error initializing the mutex object\n");
    }
}

osStatus Mutex::lock(uint32_t millisec) {
    return osMutexWait(_osMutexId, millisec);
}

bool Mutex::trylock() {
    return (osMutexWait(_osMutexId, 0) == osOK);
}

osStatus Mutex::unlock() {
    return osMutexRelease(_osMutexId);
}

}
>>>>>>> 5881ee9d9a49cdc272890e0007b9baca97e186f3
