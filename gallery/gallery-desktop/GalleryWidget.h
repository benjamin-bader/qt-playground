#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QWidget>
#include <QItemSelectionModel>

namespace Ui {
class GalleryWidget;
}

class AlbumModel;
class PictureModel;
class ThumbnailProxyModel;

class AlbumListWidget;
class AlbumWidget;

class GalleryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GalleryWidget(QWidget *parent = 0);
    ~GalleryWidget();

    void setAlbumModel(AlbumModel *albumModel);
    void setAlbumSelectionModel(QItemSelectionModel *selectionModel);

    void setPictureModel(ThumbnailProxyModel *pictureModel);
    void setPictureSelectionModel(QItemSelectionModel *selectionModel);

signals:
    void pictureActivated(const QModelIndex &index);

private:
    Ui::GalleryWidget *ui;
};

#endif // GALLERYWIDGET_H
