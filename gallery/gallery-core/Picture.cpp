#include "Picture.h"

Picture::Picture(const QString &fileName) :
    Picture(QUrl::fromLocalFile(fileName))
{
}

Picture::Picture(const QUrl &fileUrl) :
    id_(-1),
    albumId_(-1),
    fileUrl_(fileUrl)
{
}

int Picture::id() const
{
    return this->id_;
}

void Picture::setId(int id)
{
    this->id_ = id;
}

int Picture::albumId() const
{
    return this->albumId_;
}

void Picture::setAlbumId(int albumId)
{
    this->albumId_ = albumId;
}

QUrl Picture::fileUrl() const
{
    return this->fileUrl_;
}

void Picture::setFileUrl(const QUrl &fileUrl)
{
    this->fileUrl_ = fileUrl;
}
