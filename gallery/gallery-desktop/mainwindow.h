#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class GalleryWidget;
class PictureWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void displayGallery();
    void displayPicture(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    GalleryWidget *galleryWidget_;
    PictureWidget *pictureWidget_;
    QStackedWidget *stackedWidget_;
};

#endif // MAINWINDOW_H
