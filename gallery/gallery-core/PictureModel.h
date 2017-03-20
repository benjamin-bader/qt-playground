#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVariant>

#include <memory>
#include <vector>

#include "Picture.h"
#include "gallery-core_global.h"

class Album;
class AlbumModel;
class DatabaseManager;

class GALLERYCORESHARED_EXPORT PictureModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PictureRole
    {
        UrlRole = Qt::UserRole + 1,
        FilePathRole
    };

    explicit PictureModel(const AlbumModel &albumModel, QObject *parent = nullptr);

    QModelIndex addPicture(const Picture &picture);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    QHash<int, QByteArray> roleNames() const override;

    void setAlbumId(int albumId);
    void clearAlbum();

public slots:
    void deletePicturesForAlbum();

private:
    void loadPictures(int albumId);
    bool isIndexValid(const QModelIndex &index) const;

private:
    DatabaseManager &db_;
    int albumId_;
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> pictures_;
};

#endif // PICTUREMODEL_H
