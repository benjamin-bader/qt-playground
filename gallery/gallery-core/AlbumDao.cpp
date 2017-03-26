#include "AlbumDao.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>

AlbumDao::AlbumDao(QSqlDatabase &db) :
    db_(db)
{
}

void AlbumDao::init() const
{
    if (!db_.tables().contains("albums"))
    {
        QSqlQuery query(db_);
        query.exec("CREATE TABLE albums (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
    }
}

void AlbumDao::addAlbum(Album &album) const
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO albums (name) VALUES (:name)");
    query.bindValue(":name", album.name());
    query.exec();
    album.setId(query.lastInsertId().toInt());
}

void AlbumDao::updateAlbum(const Album &album) const
{
    QSqlQuery query(db_);
    query.prepare("UPDATE albums SET name = :name WHERE id = :id");
    query.bindValue(":name", album.name());
    query.bindValue(":id", album.id());
    query.exec();
}

void AlbumDao::removeAlbum(int id) const
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM albums WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
}

std::unique_ptr<std::vector<std::unique_ptr<Album>>> AlbumDao::albums() const
{
    QSqlQuery query(db_);
    query.prepare("SELECT id, name FROM albums");
    query.exec();

    std::unique_ptr<std::vector<std::unique_ptr<Album>>> result(new std::vector<std::unique_ptr<Album>>);
    while (query.next())
    {
        std::unique_ptr<Album> album(new Album());
        album->setId(query.value("id").toInt());
        album->setName(query.value("name").toString());

        result->push_back(std::move(album));
    }

    return result;
}
