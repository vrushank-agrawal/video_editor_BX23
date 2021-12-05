//
// Created by nhatv on 4 Dec 2021.
//

#ifndef VIDEO_EDITOR_BX23_IMAGEITEM_H
#define VIDEO_EDITOR_BX23_IMAGEITEM_H


#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "image.h"

using namespace img;

class ImageItem: public QObject, public QGraphicsItem
{
Q_OBJECT
public:
    explicit ImageItem(Image *image, QPointF duration, QPoint position);
    ~ImageItem();
    static double yOffset, xTimeOffset, yHeight;
    constexpr static const double border = 3;
    QPointF duration;


private:
    Image *image;
    static QBrush brush;
    static QPen pen;
    QSizeF size;
    QPointF position;
    QPixmap thumbnail;
    bool pressed=false;
    QPointF oldPos,oldMousePos;
    void calculateSize();

signals:
    void positionChanged(QPointF oldDuration, QPointF newDuration);
    void deleted(ImageItem*);

public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};


#endif //VIDEO_EDITOR_BX23_IMAGEITEM_H
