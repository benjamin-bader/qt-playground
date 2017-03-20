#ifndef THUMBNAILPROXYMODEL_H
#define THUMBNAILPROXYMODEL_H

#include <QIdentityProxyModel>
#include <QHash>
#include <QPixmap>

class PictureModel;

class ThumbnailProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    ThumbnailProxyModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &proxyIndex, int role) const override;
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    PictureModel* pictureModel() const;

private:
    void generateThumbnails(const QModelIndex &startIndex, int count);
    void reloadThumbnails();

private:
    QHash<QString, QPixmap *> thumbnails_;
};

#endif // THUMBNAILPROXYMODEL_H
