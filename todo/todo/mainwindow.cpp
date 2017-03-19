#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QInputDialog>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    p_ui(std::make_unique<Ui::MainWindow>()),
    tasks()
{
    p_ui->setupUi(this);

    connect(p_ui->addTaskButton, &QPushButton::clicked,
            this, &MainWindow::addTask);

    updateStatus();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::addTask()
{
    bool ok;
    QString name = QInputDialog::getText(
                this,
                tr("Add Task"),
                tr("Task name"),
                QLineEdit::Normal,
                tr("Untitled task"),
                &ok);

    if (ok && !name.isEmpty())
    {
        Task *t = new Task(name);
        tasks.append(t);
        p_ui->tasksLayout->addWidget(t);

        connect(t, &Task::removed, this, &MainWindow::removeTask);
        connect(t, &Task::statusChanged, this, &MainWindow::taskStatusChanged);

        updateStatus();
    }
}

void MainWindow::removeTask(Task *task)
{
    if (tasks.removeOne(task))
    {
        p_ui->tasksLayout->removeWidget(task);
        task->setParent(nullptr);
        delete task;

        updateStatus();
    }
}

void MainWindow::taskStatusChanged(Task * /*task*/)
{
    updateStatus();
}

void MainWindow::updateStatus()
{
    int numTasks = 0;
    int numComplete = 0;

    for (auto t : tasks)
    {
        ++numTasks;
        if (t->isComplete())
        {
            ++numComplete;
        }
    }

    p_ui->statusName->setText(QString("Status: %1 todo / %2 completed")
            .arg(numTasks)
            .arg(numComplete));
}
