#ifndef PICTUREDAO_H
#define PICTUREDAO_H

#include <memory>
#include <vector>

#include "Picture.h"

class QSqlDatabase;

class PictureDao
{
public:
    PictureDao(QSqlDatabase &db);

    void init() const;

    void addPictureInAlbum(int albumId, Picture &picture) const;
    void removePicture(int pictureId) const;
    void removePicturesForAlbum(int albumId) const;
    std::unique_ptr<std::vector<std::unique_ptr<Picture>>> picturesForAlbum(int albumId) const;

private:
    QSqlDatabase &db_;
};

#endif // PICTUREDAO_H
