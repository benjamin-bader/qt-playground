#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QtCharts/QLineSeries>
#include <QWidget>

#include "sysinfowidget.h"

class MemoryWidget : public SysInfoWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(QWidget *parent = nullptr);

protected slots:
    void updateSeries() override;

private:
    QtCharts::QLineSeries *series_;
    qint64 posX_;
};

#endif // MEMORYWIDGET_H
