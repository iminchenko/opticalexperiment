#pragma once

#include "constructoritem.h"

class ShieldItem : public ConstructorItem {
public:
    ShieldItem(QPointF pos, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    Properties::map getProperties() const override;
    void setProperties(const Properties::map &properties) override;

private:
    double _angle;
};
