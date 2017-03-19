#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cpuWidget_(this),
    memoryWidget_(this)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->addWidget(&cpuWidget_);
    ui->centralWidget->layout()->addWidget(&memoryWidget_);
}

MainWindow::~MainWindow()
{
    delete ui;
}
