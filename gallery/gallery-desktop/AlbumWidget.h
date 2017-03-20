#ifndef ALBUMWIDGET_H
#define ALBUMWIDGET_H

#include <QModelIndex>
#include <QWidget>

class AlbumModel;
class PictureModel;
class QItemSelectionModel;
class ThumbnailProxyModel;

namespace Ui
{
    class AlbumWidget;
}

class AlbumWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumWidget(QWidget *parent = nullptr);
    ~AlbumWidget();

    void setAlbumModel(AlbumModel *model);
    void setAlbumSelectionModel(QItemSelectionModel *selectionModel);
    void setPictureModel(ThumbnailProxyModel *model);
    void setPictureSelectionModel(QItemSelectionModel *selectionModel);

signals:
    void pictureActivated(const QModelIndex &index);

private slots:
    void deleteAlbum();
    void editAlbum();
    void addPictures();

private:
    void clearUi();
    void loadAlbum(const QModelIndex &albumIndex);

private:
    Ui::AlbumWidget *ui;

    AlbumModel *albumModel;
    QItemSelectionModel *albumSelectionModel;

    ThumbnailProxyModel *pictureModel;
    QItemSelectionModel *pictureSelectionModel;
};

#endif // ALBUMWIDGET_H
