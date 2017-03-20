#include "PictureDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

PictureDao::PictureDao(QSqlDatabase &db) :
    db_(db)
{
}

void PictureDao::init() const
{
    if (!db_.tables().contains("pictures"))
    {
        QSqlQuery query(db_);
        query.exec("CREATE TABLE pictures (id INTEGER PRIMARY KEY AUTOINCREMENT, album_id INTEGER, url TEXT");
    }
}

void PictureDao::addPictureInAlbum(int albumId, Picture &picture) const
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO pictures (album_id, url) VALUES (:album, :url)");
    query.bindValue(":album", albumId);
    query.bindValue(":url", picture.fileUrl());
    query.exec();

    picture.setId(query.lastInsertId().toInt());
}

void PictureDao::removePicture(int pictureId) const
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM pictures WHERE id = :id");
    query.bindValue(":id", pictureId);
    query.exec();
}

void PictureDao::removePicturesForAlbum(int albumId) const
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM pictures WHERE album_id = :id");
    query.bindValue(":id", albumId);
    query.exec();
}

std::unique_ptr<std::vector<std::unique_ptr<Picture> > > PictureDao::picturesForAlbum(int albumId) const
{
    QSqlQuery query(db_);
    query.prepare("SELECT id, url FROM pictures WHERE album_id = :id");
    query.bindValue(":id", albumId);

    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> result(new std::vector<std::unique_ptr<Picture>>);
    while (query.next())
    {
        std::unique_ptr<Picture> pic(new Picture());
        pic->setId(query.value("id").toInt());
        pic->setAlbumId(albumId);
        pic->setFileUrl(query.value("url").toUrl());

        result->push_back(std::move(pic));
    }

    return result;
}

