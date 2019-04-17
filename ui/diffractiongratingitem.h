#pragma once

#include "shielditem.h"

class DiffractionGratingItem : public ShieldItem
{
public:
    DiffractionGratingItem(QPointF pos, int id, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};

