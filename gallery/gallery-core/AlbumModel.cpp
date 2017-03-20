#include "AlbumModel.h"

AlbumModel::AlbumModel(QObject *parent) :
    QAbstractListModel(parent),
    db_(DatabaseManager::instance()),
    albums_(db_.albumDao.albums())
{

}

QModelIndex AlbumModel::addAlbum(const Album &album)
{
    int nextRow = rowCount();
    beginInsertRows(QModelIndex(), nextRow, nextRow);
    std::unique_ptr<Album> newAlbum(new Album(album));
    db_.albumDao.addAlbum(*newAlbum);
    albums_->push_back(std::move(newAlbum));
    endInsertRows();

    return index(nextRow, 0);
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    return albums_->size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if (! isIndexValid(index))
    {
        return QVariant();
    }

    const Album &album = *albums_->at(index.row());

    switch (role)
    {
    case Roles::IdRole:
        return album.id();

    case Roles::NameRole:
    case Qt::DisplayRole:
        return album.name();

    default:
        return QVariant();
    }
}

bool AlbumModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (! isIndexValid(index) || role != Roles::NameRole)
    {
        return false;
    }

    Album &album = *albums_->at(index.row());
    album.setName(value.toString());
    db_.albumDao.updateAlbum(album);

    emit dataChanged(index, index);

    return true;
}

bool AlbumModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0
            || row >= rowCount()
            || count < 0
            || (row + count) > rowCount())
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count);
    int remaining = count;
    while (remaining--)
    {
        const Album &album = *albums_->at(row + remaining);
        db_.albumDao.removeAlbum(album.id());
    }

    albums_->erase(albums_->begin() + row, albums_->begin() + row + count);

    endRemoveRows();
    return true;
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[Roles::IdRole] = "id";
    roleNames[Roles::NameRole] = "name";
    return roleNames;
}


bool AlbumModel::isIndexValid(const QModelIndex &index) const
{
    int row = index.row();
    return row >= 0 && row < albums_->size();
}
