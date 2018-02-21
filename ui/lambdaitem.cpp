#include "lambdaitem.h"

constexpr int DEFAULT_LAMBDA = 2;

LambdaItem::LambdaItem(QPointF pos, QGraphicsItem *parent)
    :LambdaItem(DEFAULT_LAMBDA, pos, parent) {}

LambdaItem::LambdaItem(int lambda, QPointF pos, QGraphicsItem *parent)
    :ConstructorItem(pos, parent), _lambda(lambda) {
    addInputVertex();
    addOutputVertex();
}

void LambdaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                                                                QWidget *) {
    drawBox(painter);

    painter->drawText(QPointF(-10, 5),"λ/" + QString::number(_lambda));
}
