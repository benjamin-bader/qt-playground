#include "sysinfolinux.h"

#include <sys/types.h>
#include <sys/sysinfo.h>

#include <QDebug>
#include <QFile>

SysInfoLinux::SysInfoLinux() :
    SysInfo(),
    cpuLoadLastValues_()
{
}

void SysInfoLinux::init()
{
    cpuLoadLastValues_ = cpuRawData();
}

double SysInfoLinux::memoryUsed()
{
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    qulonglong totalMemory = memInfo.totalram;
    totalMemory += memInfo.totalswap;
    totalMemory *= memInfo.mem_unit;

    qulonglong usedMemory = memInfo.totalram - memInfo.freeram;
    usedMemory += memInfo.totalswap - memInfo.freeswap;

    double percent = (double) totalMemory / (double) usedMemory * 100.0;

    return qBound(0.0, percent, 100.0);
}

double SysInfoLinux::cpuLoadAverage()
{
    auto firstSample  = cpuLoadLastValues_;
    auto secondSample = cpuRawData();

    cpuLoadLastValues_ = secondSample;

    double overall = (secondSample[0] - firstSample[0]) + (secondSample[1] - firstSample[1]) + (secondSample[2] - firstSample[2]);
    double total = overall + (secondSample[3] - firstSample[3]);
    double percent = total / overall * 100.0;

    return qBound(0.0, percent, 100.0);
}

QVector<qulonglong> SysInfoLinux::cpuRawData()
{
    QFile file("/proc/stat");
    if (! file.open(QIODevice::ReadOnly))
    {
        qFatal() << "Failed to open /proc/stat!";
    }

    QByteArray line = file.readLine();
    file.close();

    qulonglong totalUser = 0;
    qulonglong totalUserNice = 0;
    qulonglong totalSystem = 0;
    qulonglong totalIdle = 0;

    int matchedParams = std::sscanf("cpu %llu %llu %llu %llu", &totalUser, &totalUserNice, &totalSystem, &totalIdle);
    if (matchedParams != 4)
    {
        qFatal() << "/proc/stat was malformed!";
    }

    QVector<qulonglong> result;
    result.append(totalUser);
    result.append(totalUserNice);
    result.append(totalSystem);
    result.append(totalIdle);

    return result;
}
