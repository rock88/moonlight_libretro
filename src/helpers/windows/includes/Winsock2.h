// TODO: This part not include in mingw

#pragma once
#include </usr/src/mxe/usr/x86_64-w64-mingw32.static/include/winsock2.h>

#define POLLRDNORM  0x0100
#define POLLRDBAND  0x0200
#define POLLIN      (POLLRDNORM | POLLRDBAND)
#define POLLPRI     0x0400

#define POLLWRNORM  0x0010
#define POLLOUT     (POLLWRNORM)
#define POLLWRBAND  0x0020

#define POLLERR     0x0001
#define POLLHUP     0x0002
#define POLLNVAL    0x0004

typedef enum _PMTUD_STATE {
    IP_PMTUDISC_NOT_SET,
    IP_PMTUDISC_DO,
    IP_PMTUDISC_DONT,
    IP_PMTUDISC_PROBE,
    IP_PMTUDISC_MAX
} PMTUD_STATE, *PPMTUD_STATE;

#define IP_MTU_DISCOVER           71
#define AI_ADDRCONFIG               0x00000400  // Resolution only if global address configured
#define IPV6_MTU_DISCOVER           71 // Set/get path MTU discover state.

#define WLAN_API_MAKE_VERSION(_major, _minor)   (((DWORD)(_minor)) << 16 | (_major))

typedef struct pollfd {
  SOCKET fd;
  SHORT  events;
  SHORT  revents;
} WSAPOLLFD, *PWSAPOLLFD, *LPWSAPOLLFD;
