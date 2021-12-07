//
// Created by nhatv on 7 Dec 2021.
//

#include "resizer.h"

void ImageItemResizer::operator()(QGraphicsItem *item, const QRectF &rect) {
    ImageItem *imageItem = dynamic_cast<ImageItem*>(item);
    if (imageItem) {
        imageItem->setSize(QSizeF(rect.width(), rect.height()));
    }
}