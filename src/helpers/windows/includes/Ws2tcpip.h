// TODO: This part not include in mingw

#pragma once
#include </usr/src/mxe/usr/x86_64-w64-mingw32.static/include/windows.h>
#include </usr/src/mxe/usr/x86_64-w64-mingw32.static/include/ws2tcpip.h>

typedef ULONG QOS_FLOWID, *PQOS_FLOWID;

#define AI_ADDRCONFIG               0x00000400  // Resolution only if global address configured

typedef struct _QOS_VERSION
{
    USHORT MajorVersion;
    USHORT MinorVersion;
} QOS_VERSION, *PQOS_VERSION;

typedef enum _QOS_TRAFFIC_TYPE
{
    QOSTrafficTypeBestEffort        = 0,
    QOSTrafficTypeBackground        = 1,
    QOSTrafficTypeExcellentEffort   = 2,
    QOSTrafficTypeAudioVideo        = 3,
    QOSTrafficTypeVoice             = 4,
    QOSTrafficTypeControl           = 5
} QOS_TRAFFIC_TYPE, *PQOS_TRAFFIC_TYPE;

#define QOS_QUERYFLOW_FRESH         0x00000001
#define QOS_NON_ADAPTIVE_FLOW       0x00000002
