//
// Created by nhatv on 7 Dec 2021.
//

#include "imageresizer.h"

void ImageItemResizer::operator()(QGraphicsItem *item, const QRectF &rect) {
    ImageItem *imageItem = dynamic_cast<ImageItem*>(item);
    if (imageItem) {
        emit imageItem->resized(imageItem, rect.width());
    }
}