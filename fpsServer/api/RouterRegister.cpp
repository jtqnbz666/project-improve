#include "RouterFactory.h"
#include "MoveHandle.h"
#include "RouterRegister.h"

void registerAllRouters() {
    REGISTER_HANDLE("1", MoveHandle);
}
