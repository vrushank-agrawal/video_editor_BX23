//
// Created by Minh Tung Nguyen on 04/12/2021.
//

#include "indicator.h"

Indicator::Indicator(qreal height): QGraphicsItem ()
{
    pen = QPen(Qt::black,2);
    brush = QBrush(Qt::RoundCap);
    brush.setColor(QColor("#50f"));
    points<<QPointF(0,10)
          <<QPointF(-10,0)
          <<QPointF(10,0);
    setHeight(height * 2);
    setAcceptHoverEvents(true);
    this->setAcceptDrops(true);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

Indicator::~Indicator() {

}

QSizeF Indicator::calculateSize()const
{
    qreal minX = points[0].x();
    qreal minY = points[0].y();
    qreal maxX = points[0].x();
    qreal maxY = points[0].y();
    for(QPointF point : points){
        if (point.x() < minX){
            minX = point.x();
        }
        if (point.y() < minY){
            minY = point.y();
        }
        if (point.x() > maxX){
            maxX = point.x();
        }
        if (point.y() > maxY){
            maxY = point.y();
        }
    }
    return QSizeF(maxX-minX,line.p2().y());
}

QRectF Indicator::boundingRect() const
{
    QSizeF size = this->calculateSize();
    return QRectF(-10,-10,size.width(),size.height());
}

void Indicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen);
    painter->drawLine(line);
    painter->setBrush(brush);
    painter->drawPolygon(&points[0], points.size());
}

void Indicator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    qDebug()<<"press";

    QGraphicsItem::mousePressEvent(event);
    update();
}

void Indicator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();
    qDebug()<<"move";
    if(pressed){
        this->setPos(pos.x(),y());
    }
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void Indicator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    qDebug()<<event->scenePos();
    emit positionChanged(event->scenePos().x());
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

QVariant Indicator::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        newPos.setY(y());
        if(newPos.x() < 0){
            newPos.setX(0);
        }
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}