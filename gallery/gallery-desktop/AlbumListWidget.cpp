#include "AlbumListWidget.h"
#include "ui_AlbumListWidget.h"

#include <QDebug>
#include <QInputDialog>

#include "AlbumModel.h"

AlbumListWidget::AlbumListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumListWidget),
    albumModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->createAlbumButton, &QPushButton::clicked,
            this, &AlbumListWidget::createAlbum);
}

AlbumListWidget::~AlbumListWidget()
{
    delete ui;
}

void AlbumListWidget::setModel(AlbumModel *model)
{
    this->albumModel = model;
    ui->albumList->setModel(this->albumModel);
}

void AlbumListWidget::setSelectionModel(QItemSelectionModel *itemSelectionModel)
{
    ui->albumList->setSelectionModel(itemSelectionModel);
}

void AlbumListWidget::createAlbum()
{
    if (albumModel == nullptr)
    {
        qDebug() << "Album model is null, cannot create album";
        return;
    }

    bool ok;
    QString albumName = QInputDialog::getText(
                this,
                tr("Create a new Album"),
                tr("Choose a name"),
                QLineEdit::Normal,
                tr("New album"),
                &ok);

    if (ok && !albumName.isEmpty())
    {
        Album album(albumName);
        QModelIndex index = albumModel->addAlbum(album);
        ui->albumList->setCurrentIndex(index);
    }
}
