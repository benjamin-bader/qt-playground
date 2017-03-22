#include "AlbumWidget.h"
#include "ui_albumwidget.h"

#include <QFileDialog>
#include <QInputDialog>

#include "AlbumModel.h"
#include "PictureDelegate.h"
#include "PictureModel.h"
#include "ThumbnailProxyModel.h"

AlbumWidget::AlbumWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumWidget),
    albumModel(nullptr),
    albumSelectionModel(nullptr),
    pictureModel(nullptr),
    pictureSelectionModel(nullptr)
{
    ui->setupUi(this);
    clearUi();

    ui->thumbnailListView->setSpacing(5);
    ui->thumbnailListView->setResizeMode(QListView::Adjust);
    ui->thumbnailListView->setFlow(QListView::LeftToRight);
    ui->thumbnailListView->setWrapping(true);
    ui->thumbnailListView->setItemDelegate(new PictureDelegate(this));

    connect(ui->thumbnailListView, &QListView::doubleClicked, this, &AlbumWidget::pictureActivated);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AlbumWidget::deleteAlbum);
    connect(ui->editButton, &QPushButton::clicked, this, &AlbumWidget::editAlbum);
    connect(ui->addPicturesButton, &QPushButton::clicked, this, &AlbumWidget::addPictures);
}

AlbumWidget::~AlbumWidget()
{
    delete ui;
}

void AlbumWidget::setAlbumModel(AlbumModel *model)
{
    this->albumModel = model;

    connect(this->albumModel, &QAbstractItemModel::dataChanged,
            [this] (const QModelIndex &topLeft) {
        if (albumSelectionModel && topLeft == albumSelectionModel->currentIndex())
        {
            loadAlbum(topLeft);
        }
    });
}

void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel *selectionModel)
{
    this->albumSelectionModel = selectionModel;

    connect(this->albumSelectionModel, &QItemSelectionModel::selectionChanged,
            [this] (const QItemSelection &selected) {
        if (selected.isEmpty())
        {
            clearUi();
            return;
        }

        loadAlbum(selected.indexes().first());
    });
}

void AlbumWidget::setPictureModel(ThumbnailProxyModel *pictureModel)
{
    this->pictureModel = pictureModel;
    ui->thumbnailListView->setModel(pictureModel);
}

void AlbumWidget::setPictureSelectionModel(QItemSelectionModel *selectionModel)
{
    this->pictureSelectionModel = selectionModel;
    ui->thumbnailListView->setSelectionModel(selectionModel);
}

void AlbumWidget::deleteAlbum()
{
    if (! albumModel || ! albumSelectionModel || albumSelectionModel->selectedIndexes().isEmpty())
    {
        return;
    }

    int row = albumSelectionModel->currentIndex().row();
    albumModel->removeRow(row);

    QModelIndex previousIndex = albumModel->index(row - 1, 0);
    if (previousIndex.isValid())
    {
        albumSelectionModel->setCurrentIndex(previousIndex, QItemSelectionModel::SelectCurrent);
        return;
    }

    QModelIndex nextIndex = albumModel->index(row + 1, 0);
    if (previousIndex.isValid())
    {
        albumSelectionModel->setCurrentIndex(previousIndex, QItemSelectionModel::SelectCurrent);
        return;
    }
}

void AlbumWidget::editAlbum()
{
    if (! albumModel || ! albumSelectionModel || albumSelectionModel->selectedIndexes().isEmpty())
    {
        return;
    }

    QModelIndex albumIndex = albumSelectionModel->selectedIndexes().first();
    QString currentAlbumName = albumModel->data(albumIndex, AlbumModel::Roles::NameRole).toString();

    bool ok;
    QString newAlbumName = QInputDialog::getText(
                this,
                tr("Rename Album"),
                tr("Change Album Name"),
                QLineEdit::Normal,
                currentAlbumName,
                &ok);

    if (ok && ! newAlbumName.isEmpty())
    {
        albumModel->setData(albumIndex, newAlbumName, AlbumModel::Roles::NameRole);
    }
}

void AlbumWidget::addPictures()
{
    if (! albumModel || ! pictureModel)
    {
        return;
    }

    QStringList fileNames = QFileDialog::getOpenFileNames(
                this,
                tr("Add pictures"),
                QDir::homePath(),
                "Picture files (*.jpg *.png)");

    if (! fileNames.isEmpty())
    {
        QModelIndex lastIndex;
        for (auto fileName : fileNames)
        {
            Picture picture(fileName);
            lastIndex = pictureModel->pictureModel()->addPicture(picture);
        }

        QModelIndex proxyModelIndex = pictureModel->index(lastIndex.row(), lastIndex.column());
        ui->thumbnailListView->setCurrentIndex(proxyModelIndex);
    }
}

void AlbumWidget::clearUi()
{
    ui->albumName->setText("");
    ui->deleteButton->setVisible(false);
    ui->editButton->setVisible(false);
    ui->addPicturesButton->setVisible(false);
}

void AlbumWidget::loadAlbum(const QModelIndex &albumIndex)
{
    if (! albumModel || ! pictureModel)
    {
        return;
    }

    int albumId = albumModel->data(albumIndex, AlbumModel::Roles::IdRole).toInt();
    pictureModel->pictureModel()->setAlbumId(albumId);

    ui->albumName->setText(albumModel->data(albumIndex, Qt::DisplayRole).toString());

    ui->deleteButton->setVisible(true);
    ui->editButton->setVisible(true);
    ui->addPicturesButton->setVisible(true);
}
