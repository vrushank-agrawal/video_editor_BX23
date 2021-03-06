//
// Created by nhatv on 24 Nov 2021.
//

#include "imagemanager.h"

ImageManager::ImageManager(QListWidget *qListWidget) : listWidget(qListWidget) {
    qListWidget->setDragEnabled(true);
    qListWidget->setViewMode(QListView::IconMode);
    qListWidget->setIconSize(QSize(50, 50));
    qListWidget->setGridSize(QSize(80, 90));
    qListWidget->setSpacing(20);
    qListWidget->setContentsMargins(20, 20, 20, 20);
    qListWidget->setMovement(QListView::Snap);
    qListWidget->setAcceptDrops(true);
    qListWidget->setDropIndicatorShown(true);
    qListWidget->setWordWrap(true);
    qListWidget->setUniformItemSizes(true);
    qListWidget->setAutoScrollMargin(100);

    brush = QBrush("#dddddd");
    map = QMap<QListWidgetItem*, img::Image>();
}

void ImageManager::addImage(img::Image image, const QString& name) {
    cv::Mat mat = image.getMat();
    QImage qImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    addImage(
            QPixmap::fromImage(qImage.rgbSwapped()),
            name,
            &image
            );
}

void ImageManager::addImage(const QPixmap& image, const QString& name, img::Image *img) {
    QString displayName;
    if (name.lastIndexOf("/") >= 0) {
        displayName = name.right(name.length() - name.lastIndexOf("/") - 1);
    }
    else {
        displayName = name;
    }

    auto *item = new QListWidgetItem(QIcon(image),displayName);
    if (img != nullptr)
        map.insert(item, *img);
    item->setBackground(brush);
    item->setSizeHint(QSize(60, 70));
    listWidget->addItem(item);
}

ImageManager::~ImageManager() {
    delete listWidget;
}

int ImageManager::getImagesCount() {
    return map.size();
}

img::Image *ImageManager::getImage(QListWidgetItem *item) {
    return &map.find(item).value();
}

img::Image *ImageManager::getImage(int row) {
    if (row >= getImagesCount() || row < 0) return nullptr;
    return getImage(listWidget->item(row));
}
