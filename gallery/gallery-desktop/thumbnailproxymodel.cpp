#include "ThumbnailProxyModel.h"

#include <QDebug>

#include "PictureModel.h"

const unsigned int THUMBNAIL_SIZE = 350;

ThumbnailProxyModel::ThumbnailProxyModel(QObject *parent) :
    QIdentityProxyModel(parent),
    thumbnails_()
{

}

void ThumbnailProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QIdentityProxyModel::setSourceModel(sourceModel);
    if (! sourceModel)
    {
        return;
    }

    connect(sourceModel, &QAbstractItemModel::modelReset, [this] { reloadThumbnails(); });
    connect(sourceModel, &QAbstractItemModel::rowsInserted,
            [this] (const QModelIndex & /* parent */, int first, int last) {
        generateThumbnails(index(first, 0), last - first + 1);
    });
}

QVariant ThumbnailProxyModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (role != Qt::DecorationRole)
    {
        return QIdentityProxyModel::data(proxyIndex, role);
    }

    QString filePath = sourceModel()->data(proxyIndex, PictureModel::PictureRole::FilePathRole).toString();
    return *thumbnails_[filePath];
}

PictureModel* ThumbnailProxyModel::pictureModel() const
{
    return static_cast<PictureModel *>(sourceModel());
}

void ThumbnailProxyModel::generateThumbnails(const QModelIndex &startIndex, int count)
{
    if (!startIndex.isValid())
    {
        return;
    }

    const QAbstractItemModel *model = startIndex.model();

    int lastRow = startIndex.row() + count;
    for (int row = startIndex.row(); row < lastRow; ++row)
    {
        QString filePath = model->data(index(row, 0), PictureModel::PictureRole::FilePathRole).toString();
        QPixmap pixmap(filePath);

        auto thumbnail = new QPixmap(pixmap.scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        thumbnails_.insert(filePath, thumbnail);
    }
}

void ThumbnailProxyModel::reloadThumbnails()
{
    qDeleteAll(thumbnails_);
    thumbnails_.clear();
    generateThumbnails(index(0, 0), rowCount());
}
