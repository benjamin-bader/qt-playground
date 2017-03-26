#include "PictureDao.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
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
        query.exec("CREATE TABLE pictures (id INTEGER PRIMARY KEY AUTOINCREMENT, album_id INTEGER, url TEXT)");
    }
}

void PictureDao::addPictureInAlbum(int albumId, Picture &picture) const
{
    QSqlQuery query(db_);
    bool didPrepare = query.prepare("INSERT INTO pictures (album_id, url) VALUES (:album , :url)");
    if (! didPrepare)
    {
        qDebug() << "Error preparing query: " << query.lastError().text();
        return;
    }

    query.bindValue(":album", albumId);
    query.bindValue(":url", picture.fileUrl().toString());
    bool queryExecuted = query.exec();

    if (! queryExecuted)
    {
        qCritical() << "Error: " << query.lastError().text();
    }

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
    query.setForwardOnly(true);

    if (!query.prepare("SELECT id, url FROM pictures WHERE album_id = :id"))
    {
        qCritical() << "Failed to prepare pic query";
        qCritical() << query.lastError().text();
    }
    query.bindValue(":id", albumId);

    if (! query.exec())
    {
        qCritical() << "Failed to execute pic query";
        qCritical() << query.lastError().text();
    }

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

