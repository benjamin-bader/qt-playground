#ifndef ALBUMLISTWIDGET_H
#define ALBUMLISTWIDGET_H

#include <QItemSelectionModel>
#include <QWidget>

#include "GalleryWidget.h"

namespace Ui {
class AlbumListWidget;
}

class AlbumModel;

class AlbumListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumListWidget(QWidget *parent = nullptr);
    ~AlbumListWidget() override;

    void setModel(AlbumModel *model);
    void setSelectionModel(QItemSelectionModel *itemSelectionModel);

private slots:
    void createAlbum();

private:
    Ui::AlbumListWidget *ui;
    AlbumModel *albumModel;
};

#endif // ALBUMLISTWIDGET_H
