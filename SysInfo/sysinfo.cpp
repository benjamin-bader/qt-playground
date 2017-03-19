#include "sysinfo.h"

#include <mutex>

#include <QtGlobal>

#ifdef Q_OS_WIN
    #include "sysinfowindows.h"
#elif defined(Q_OS_MAC)
    #include "sysinfomac.h"
#elif defined(Q_OS_LINUX)
    #include "sysinfolinux.h"
#else
    #error "Neither Windows, Mac, or Linux - unsupported OS"
#endif

SysInfo& SysInfo::instance()
{
#ifdef Q_OS_WIN
    static SysInfoWindows instance;
#elif defined(Q_OS_MAC)
    static SysInfoMac instance;
#elif defined(Q_OS_LINUX)
    static SysInfoLinux instance;
#else
    #error "Neither Windows, Mac, or Linux - unsupported OS"
#endif

    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        instance.init();
    });

    return instance;
}

SysInfo::SysInfo()
{
}

SysInfo::~SysInfo()
{
}
