//
// Created by nhatv on 2 Jan 2022.
//

#include "imageresizer.h"
#include "timeline.h"

double Timeline::default_image_length = 5;

void Timeline::addImage(img::Image *image, double start, double end) {
    auto *item = new ImageItem(image, QPoint(start * xTimeOffset, ImageItem::border));
    item->start = imageMap.insert(start, item);
    item->end = imageMap.insert(end, nullptr);
    item->calculateSize();
    scene->addItem(item);

    connect(item, &ImageItem::itemMoved,
            this, &Timeline::moveImageItem);
    connect(item, &ImageItem::positionChanged,
            this, &Timeline::updateImagePosition);
    connect(item, &ImageItem::resized,
            this, &Timeline::resizeImageItem);
    connect(item, &ImageItem::deleted,
            this, &Timeline::deleteImage);

    connect(item, &ImageItem::animationApplied,
            this, &Timeline::animationApplied);
    connect(item, &ImageItem::imageSelected,
            this, &Timeline::imageSelected);
    connect(item, &ImageItem::blurTypeApplied,
            this, &Timeline::blurTypeApplied);

    auto temp = new SizeGripItem(new ImageItemResizer, item);
    temp->setMaxWidth(std::numeric_limits<double>::infinity());
    item->createSizeGripItem(temp);

    if (end > lengthInSecond) {
        int newEnd = ceil(end / 5) * 5;
        updateVideoLength(newEnd + 30);
    }

    emit imageAdded(image, start, end-start, vid::Normal);
}

void Timeline::addImageAtIndicator(img::Image *image, double max_length) {
    double time = indicator->x() / xTimeOffset;

    // image already exists
    if (getImage(time) != nullptr) {
        appendImage(image, max_length);
        return;
    }

    QMultiMap<double, ImageItem*>::iterator end = imageMap.upperBound(time);
    double duration;
    if (end == imageMap.end()) // trying to append image at the end of timeline
        duration = max_length;
    else
        duration = (end.key() - time > max_length) ? max_length : end.key() - time;
    addImage(image, time, time + duration);
}

void Timeline::appendImage(img::Image *image, double length) {
    double start = imageMap.isEmpty() ? 0 : imageMap.lastKey();
    addImage(image, start, start + length);
}

void Timeline::deleteImage(ImageItem *item) {
    imageMap.erase(item->start);
    imageMap.erase(item->end);
    emit imageDeleted(item->image);
}

img::Image* Timeline::getImage(double time) {
    ImageItem *item = getImageItem(time);
    if (item != nullptr) return item->image;
    return nullptr;
}

ImageItem* Timeline::getImageItem(double time) {
    for (auto iterator = imageMap.begin(); iterator != imageMap.end(); iterator++) {
        if (iterator.value() == nullptr) continue;
        double startTime = iterator.value()->start.key(), endTime = iterator.value()->end.key();

        if (startTime <= time + eps && time - eps <= endTime) return iterator.value();
    }

    return nullptr;
}

void Timeline::moveImageItem(ImageItem *item, double startPos, double endPos) {
    double startTime = startPos / xTimeOffset;
    double endTime = endPos / xTimeOffset;

    // detect collision with other images
    QMultiMap<double, ImageItem*>::iterator iterator = imageMap.lowerBound(startTime);
    while (iterator != imageMap.end() && iterator.key() < endTime) {
        if (iterator.value() != nullptr && iterator.value() != item) {
            setImageItemPosition(item, startTime + iterator.key() - endTime, iterator.key());
            return;
        }
        iterator++;
    }

    setImageItemPosition(item, startTime, endTime);
}

void Timeline::setImageItemPosition(ImageItem *item, double startTime, double endTime) {
    if (startTime < 0) return;
    for (auto iterator = imageMap.begin(); iterator != imageMap.end(); iterator++) {
        if (iterator.value() == nullptr || iterator.value() == item) continue;
        double start = iterator.value()->start.key(), end = iterator.value()->end.key();

        if (startTime < end && start < endTime) return;
    }
    item->setX(startTime * xTimeOffset);
}

void Timeline::resizeImageItem(ImageItem *item, double newLength) {
    double startTime = item->x() / xTimeOffset;
    double endTime = (item->x() + newLength) / xTimeOffset;

    // detect collision with other images
    QMultiMap<double, ImageItem*>::iterator iterator = imageMap.lowerBound(startTime);
    while (iterator != imageMap.end() && iterator.key() < endTime) {
        if (iterator.value() != nullptr && iterator.value() != item) {
            item->updateDuration((iterator.key() - startTime) * xTimeOffset);
            return;
        }
        iterator++;
    }
    item->updateDuration(newLength);
}

void Timeline::updateImagePosition(ImageItem* item, double start, double end) {
    if (item->start.key() == start && item->end.key() == end)
        return;
    // delete old duration
    deleteImage(item);

    // add new duration
    item->start = imageMap.insert(start, item);
    item->end = imageMap.insert(end, nullptr);

    if (end > lengthInSecond) {
        int newEnd = ceil(end / 5) * 5;
        updateVideoLength(newEnd + 30);
    }

    emit imageAdded(item->image, start, end-start, item->animation);
}