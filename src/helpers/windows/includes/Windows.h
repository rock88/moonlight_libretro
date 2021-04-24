// TODO: This part not include in mingw

#pragma once
#include </usr/src/mxe/usr/x86_64-w64-mingw32.static/include/windows.h>

#define CREATE_EVENT_MANUAL_RESET   0x00000001

#define CreateEventEx  CreateEventExA
#define CreateMutexEx  CreateMutexExA
