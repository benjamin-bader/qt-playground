#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Task.h"

#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addTask();
    void removeTask(Task *task);
    void taskStatusChanged(Task *task);

private:
    void updateStatus();

    std::unique_ptr<Ui::MainWindow> p_ui;

    QVector<Task *> tasks;
};

#endif // MAINWINDOW_H
