#include "PictureWidget.h"
#include "ui_PictureWidget.h"

#include <QDebug>

#include "PictureModel.h"
#include "ThumbnailProxyModel.h"


PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget),
    model_(nullptr),
    selectionModel_(nullptr),
    pixmap_()
{
    ui->setupUi(this);
    ui->pictureLabel->setMinimumSize(1, 1);

    connect(ui->backButton, &QPushButton::clicked, this, &PictureWidget::backToGallery);
    connect(ui->deleteButton, &QPushButton::clicked, this, &PictureWidget::deletePicture);

    connect(ui->previousButton, &QPushButton::clicked,
            [this] () {
        if (! selectionModel_)
        {
            qFatal("No selection model set!");
            return;
        }
        QModelIndex currentIndex = selectionModel_->currentIndex();
        QModelIndex previousIndex = selectionModel_->model()->index(currentIndex.row() - 1, 0);
        selectionModel_->setCurrentIndex(previousIndex, QItemSelectionModel::Current);
    });

    connect(ui->nextButton, &QPushButton::clicked,
            [this] () {
        if (! selectionModel_)
        {
            qFatal("No selection model set!");
            return;
        }
        QModelIndex currentIndex = selectionModel_->currentIndex();
        QModelIndex nextIndex = selectionModel_->model()->index(currentIndex.row() + 1, 0);
        selectionModel_->setCurrentIndex(nextIndex, QItemSelectionModel::Current);
    });
}

PictureWidget::~PictureWidget()
{
    delete ui;
}

void PictureWidget::setModel(ThumbnailProxyModel *model)
{
    this->model_ = model;
}

void PictureWidget::setSelectionModel(QItemSelectionModel *selectionModel)
{
    this->selectionModel_ = selectionModel;
    if (! selectionModel)
    {
        return;
    }

    connect(selectionModel, &QItemSelectionModel::selectionChanged,
            this, &PictureWidget::loadPicture);
}

void PictureWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updatePicturePixmap();
}

void PictureWidget::deletePicture()
{
    if (! model_)
    {
        qFatal("model_ is not set");
        return;
    }

    if (! selectionModel_)
    {
        qFatal("selectionModel_ is not set");
        return;
    }

    int row = selectionModel_->currentIndex().row();
    model_->removeRow(row);

    int priorRow = qMax(0, row - 1);
    QModelIndex priorIndex = model_->index(priorRow, 0);

    if (priorIndex.isValid())
    {
        selectionModel_->setCurrentIndex(priorIndex, QItemSelectionModel::Current);
        return;
    }

    int nextRow = row + 1;
    QModelIndex nextIndex = model_->index(nextRow, 0);
    if (nextIndex.isValid())
    {
        selectionModel_->setCurrentIndex(nextIndex, QItemSelectionModel::Current);
        return;
    }

    emit backToGallery();
}

void PictureWidget::loadPicture(const QItemSelection &selected)
{
    if (selected.indexes().isEmpty())
    {
        ui->nameLabel->setText("");
        ui->pictureLabel->setPixmap(QPixmap());
        ui->deleteButton->setEnabled(false);
        return;
    }

    QModelIndex current = selected.indexes().first();

    pixmap_ = QPixmap(model_->data(current, PictureModel::PictureRole::FilePathRole).toString());
    updatePicturePixmap();

    ui->nameLabel->setText(model_->data(current, Qt::DisplayRole).toString());
    ui->previousButton->setEnabled(current.row() > 0);
    ui->nextButton->setEnabled(current.row() < model_->rowCount() - 1);
    ui->deleteButton->setEnabled(true);
}

void PictureWidget::updatePicturePixmap()
{
    if (pixmap_.isNull())
    {
        return;
    }

    ui->pictureLabel->setPixmap(pixmap_.scaled(ui->pictureLabel->size(), Qt::KeepAspectRatio));
}
