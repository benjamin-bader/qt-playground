#ifndef ALBUMDAO_H
#define ALBUMDAO_H

#include <memory>
#include <vector>

#include "Album.h"

class QSqlDatabase;

class AlbumDao
{
public:
    AlbumDao(QSqlDatabase &db);
    void init() const;

    void addAlbum(Album &album) const;
    void updateAlbum(const Album &album) const;
    void removeAlbum(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Album>>> albums() const;

private:
    QSqlDatabase &db_;
};

#endif // ALBUMDAO_H
