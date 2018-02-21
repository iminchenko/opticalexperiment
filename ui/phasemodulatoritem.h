#pragma once

#include "constructoritem.h"

class PhaseModulatorItem : public ConstructorItem {
public:
    PhaseModulatorItem(QPointF pos, QGraphicsItem *parent = nullptr);

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};
