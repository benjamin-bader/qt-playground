#include "sysinfowindows.h"

#include <windows.h>

#include <QDebug>

SysInfoWindows::SysInfoWindows() :
    SysInfo(),
    cpuLoadLastValues_()
{
}

void SysInfoWindows::init()
{
    cpuLoadLastValues_ = cpuRawData();
}

double SysInfoWindows::memoryUsed()
{
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (! GlobalMemoryStatusEx(&memoryStatus))
    {
        qFatal("Failed to query memory status!");
    }

    qlonglong physicalMemoryUsed = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;

    return (double) physicalMemoryUsed / (double) memoryStatus.ullTotalPhys * 100.0;
}

double SysInfoWindows::cpuLoadAverage()
{
    auto firstSample  = cpuLoadLastValues_;
    auto secondSample = cpuRawData();

    cpuLoadLastValues_ = secondSample;

    auto currentIdle   = secondSample[0] - firstSample[0];
    auto currentKernel = secondSample[1] - firstSample[1];
    auto currentUser   = secondSample[2] - firstSample[2];
    auto currentSystem = currentKernel + currentUser;

    double percent = (currentSystem - currentIdle) * 100.0 / currentSystem;

    if (isnan(percent))
    {
        percent = 0.0;
    }

    return qBound(0.0, percent, 100.0);
}

QVector<qulonglong> SysInfoWindows::cpuRawData()
{
    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    GetSystemTimes(&idleTime, &kernelTime, &userTime);

    QVector<qulonglong> rawData;
    rawData.append(convertFileTime(idleTime));
    rawData.append(convertFileTime(kernelTime));
    rawData.append(convertFileTime(userTime));

    return rawData;
}

qulonglong SysInfoWindows::convertFileTime(const FILETIME &filetime) const
{
    ULARGE_INTEGER largeInteger;
    largeInteger.LowPart = filetime.dwLowDateTime;
    largeInteger.HighPart = filetime.dwHighDateTime;
    return largeInteger.QuadPart;
}
