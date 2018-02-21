#pragma once

#include "constructoritem.h"

class LambdaItem : public ConstructorItem {
public:
    LambdaItem(QPointF pos, QGraphicsItem *parent = nullptr);
    LambdaItem(int lambda, QPointF pos, QGraphicsItem *parent = nullptr);


protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
    int _lambda;
};
