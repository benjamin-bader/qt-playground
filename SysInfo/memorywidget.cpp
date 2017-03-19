#include "memorywidget.h"

#include <QtCharts/QAreaSeries>

#include "sysinfo.h"

using namespace QtCharts;

const int CHART_X_RANGE_COUNT = 50;
const int CHART_X_RANGE_MAX = CHART_X_RANGE_COUNT - 1;

MemoryWidget::MemoryWidget(QWidget *parent) :
    SysInfoWidget(parent),
    series_(new QLineSeries(this)),
    posX_(0)
{
    QAreaSeries *areaSeries = new QAreaSeries(series_);

    QChart *chart = chartView().chart();
    chart->addSeries(areaSeries);
    chart->setTitle("Memory used");
    chart->createDefaultAxes();
    chart->axisX()->setVisible(false);
    chart->axisX()->setRange(0, CHART_X_RANGE_MAX);
    chart->axisY()->setRange(0, 100);
}

void MemoryWidget::updateSeries()
{
    double memoryUsed = SysInfo::instance().memoryUsed();
    series_->append(posX_++, memoryUsed);
    if (series_->count() > CHART_X_RANGE_COUNT)
    {
        QChart *chart = chartView().chart();
        chart->scroll(chart->plotArea().width() / CHART_X_RANGE_MAX, 0);
        series_->remove(0);
    }
}
