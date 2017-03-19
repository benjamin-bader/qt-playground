#include "sysinfowidget.h"

#include <QVBoxLayout>

using namespace QtCharts;

SysInfoWidget::SysInfoWidget(QWidget *parent, int startDelayMs, int updateSeriesDelayMs) :
    QWidget(parent),
    chartView_(this)

{
    refreshTimer_.setInterval(updateSeriesDelayMs);
    connect(&refreshTimer_, &QTimer::timeout, this, &SysInfoWidget::updateSeries);
    QTimer::singleShot(startDelayMs, [this] { this->refreshTimer_.start(); });

    chartView_.setRenderHint(QPainter::Antialiasing);
    chartView_.chart()->legend()->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(&chartView_);
    setLayout(layout);
}

QChartView& SysInfoWidget::chartView()
{
    return chartView_;
}
