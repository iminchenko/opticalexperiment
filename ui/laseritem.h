#include "constructoritem.h"

class LaserItem : public ConstructorItem {
public:
    LaserItem(QPointF pos, QGraphicsItem *parent = nullptr);

    Properties::map getProperties() const override;
    void setProperties(const Properties::map &properties) override;

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    double _frequency;
};
