#include "cpuwidget.h"

#include "sysinfo.h"

using namespace QtCharts;

CpuWidget::CpuWidget(QWidget *parent) :
    SysInfoWidget(parent),
    series_(new QPieSeries(this))
{
    series_->setHoleSize(0.35);
    series_->append("CPU Load", 30.0);
    series_->append("CPU Free", 70.0);

    QChart *chart = chartView().chart();
    chart->addSeries(series_);
    chart->setTitle("CPU average load");
}

void CpuWidget::updateSeries()
{
    double cpuLoadAverage = SysInfo::instance().cpuLoadAverage();
    series_->clear();
    series_->append("Load", cpuLoadAverage);
    series_->append("Free", 100.0 - cpuLoadAverage);
}
