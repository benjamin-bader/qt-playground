#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include "sysinfowidget.h"

#include <QWidget>
#include <QtCharts/QPieSeries>

class CpuWidget : public SysInfoWidget
{
    Q_OBJECT
public:
    explicit CpuWidget(QWidget *parent = nullptr);

protected slots:
    void updateSeries() override;

private:
    QtCharts::QPieSeries *series_;
};

#endif // CPUWIDGET_H
