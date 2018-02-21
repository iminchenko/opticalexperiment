#pragma once

#include "constructoritem.h"

class LightbulbItem : public ConstructorItem {
public:
    LightbulbItem(QPointF pos, QGraphicsItem *parent = nullptr);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};
