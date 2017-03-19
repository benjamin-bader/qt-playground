#ifndef SYSINFOWINDOWS_H
#define SYSINFOWINDOWS_H

#include "sysinfo.h"

#include <QtGlobal>
#include <QVector>

typedef struct _FILETIME FILETIME;

class SysInfoWindows : public SysInfo
{
public:
    SysInfoWindows();

public:
    void init() override;
    double cpuLoadAverage() override;
    double memoryUsed() override;

private:
    QVector<qulonglong> cpuRawData();
    qulonglong convertFileTime(const FILETIME& filetime) const;

private:
    QVector<qulonglong> cpuLoadLastValues_;
};

#endif // SYSINFOWINDOWS_H
