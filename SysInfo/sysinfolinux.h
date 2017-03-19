#ifndef SYSINFOLINUX_H
#define SYSINFOLINUX_H

#include <QtGlobal>
#include <QVector>

#include "sysinfo.h"

class SysInfoLinux : public SysInfo
{
public:
    SysInfoLinux();

public:
    void init() override;
    double cpuLoadAverage() override;
    double memoryUsed() override;

private:
    QVector<qulonglong> cpuRawData();

private:
    QVector<qulonglong> cpuLoadLastValues_;
};

#endif // SYSINFOLINUX_H
