#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AlbumModel.h"
#include "PictureModel.h"

#include "GalleryWidget.h"
#include "PictureWidget.h"
#include "ThumbnailProxyModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    galleryWidget_(new GalleryWidget(this)),
    pictureWidget_(new PictureWidget(this)),
    stackedWidget_(new QStackedWidget(this))
{
    ui->setupUi(this);

    AlbumModel *albumModel = new AlbumModel(this);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(albumModel, this);
    galleryWidget_->setAlbumModel(albumModel);
    galleryWidget_->setAlbumSelectionModel(selectionModel);

    PictureModel *pictureModel = new PictureModel(this);
    ThumbnailProxyModel *thumbnailModel = new ThumbnailProxyModel(this);
    thumbnailModel->setSourceModel(pictureModel);

    QItemSelectionModel *pictureSelectionModel = new QItemSelectionModel(thumbnailModel, this);

    galleryWidget_->setPictureModel(thumbnailModel);
    galleryWidget_->setPictureSelectionModel(pictureSelectionModel);

    pictureWidget_->setModel(thumbnailModel);
    pictureWidget_->setSelectionModel(pictureSelectionModel);

    connect(galleryWidget_, &GalleryWidget::pictureActivated, this, &MainWindow::displayPicture);
    connect(pictureWidget_, &PictureWidget::backToGallery,    this, &MainWindow::displayGallery);

    stackedWidget_->addWidget(galleryWidget_);
    stackedWidget_->addWidget(pictureWidget_);
    displayGallery();

    setCentralWidget(stackedWidget_);
}

MainWindow::~MainWindow()
{
    delete stackedWidget_;
    delete ui;
}

void MainWindow::displayGallery()
{
    stackedWidget_->setCurrentWidget(galleryWidget_);
}

void MainWindow::displayPicture(const QModelIndex &index)
{
    stackedWidget_->setCurrentWidget(pictureWidget_);
}
