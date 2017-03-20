#include "PictureModel.h"

#include "AlbumModel.h"
#include "DatabaseManager.h"

PictureModel::PictureModel(const AlbumModel &album, QObject *parent) :
    QAbstractListModel(parent),
    db_(DatabaseManager::instance()),
    albumId_(-1),
    pictures_(new std::vector<std::unique_ptr<Picture>>)
{
    connect(&album, &AlbumModel::rowsRemoved, this, &PictureModel::deletePicturesForAlbum);
}

void PictureModel::setAlbumId(int albumId)
{
    beginResetModel();
    albumId_ = albumId;
    loadPictures(albumId_);
    endResetModel();
}

QModelIndex PictureModel::addPicture(const Picture &picture)
{
    int rows = rowCount();
    beginInsertRows(QModelIndex(), rows, rows);
    std::unique_ptr<Picture> newPicture(new Picture(picture));
    db_.pictureDao.addPictureInAlbum(albumId_, *newPicture);
    pictures_->push_back(std::move(newPicture));
    endInsertRows();

    return index(rows, 0);
}

void PictureModel::loadPictures(int albumId)
{
    if (albumId < 0)
    {
        pictures_.reset(new std::vector<std::unique_ptr<Picture>>);
        return;
    }

    pictures_ = db_.pictureDao.picturesForAlbum(albumId);
}

int PictureModel::rowCount(const QModelIndex & /* parent */) const
{
    return pictures_->size();
}

QVariant PictureModel::data(const QModelIndex &index, int role) const
{
    if (! isIndexValid(index))
    {
        return QVariant();
    }

    const Picture &picture = *pictures_->at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
        return picture.fileUrl().fileName();

    case PictureRole::UrlRole:
        return picture.fileUrl();

    case PictureRole::FilePathRole:
        return picture.fileUrl().toLocalFile();

    default:
        return QVariant();
    }
}

bool PictureModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (! isIndexValid(index)
            || role != PictureRole::FilePathRole
            || value.userType() != QMetaType::QUrl)
    {
        return false;
    }

    Picture &pic = *pictures_->at(index.row());
    pic.setFileUrl(value.toUrl());

    emit dataChanged(index, index);

    return true;
}

bool PictureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count < 0 || row + count > rowCount(parent))
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count);

    int remaining = count;
    while (remaining--)
    {
        const Picture &pic = *pictures_->at(row + remaining);
        db_.pictureDao.removePicture(pic.id());
    }

    pictures_->erase(pictures_->begin() + row, pictures_->begin() + row + count);

    endRemoveRows();
    return true;
}

void PictureModel::deletePicturesForAlbum()
{
    if (albumId_ >= 0)
    {
        db_.pictureDao.removePicturesForAlbum(albumId_);
    }
}

QHash<int, QByteArray> PictureModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[PictureRole::UrlRole] = "url";
    names[PictureRole::FilePathRole] = "filepath";
    return names;
}

bool PictureModel::isIndexValid(const QModelIndex &index) const
{
    int row = index.row();
    return row >= 0 && row < rowCount() && index.isValid();
}

