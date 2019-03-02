#pragma once

#include <QGraphicsItem>
#include <array>

class ConnectionItem;
class VertexItem;
class InputVertexItem;
class OutputVertexItem;

class ConnectionItem : public QGraphicsItem {
friend class VertexItem;
public:
    ConnectionItem(OutputVertexItem *source,
                   InputVertexItem *destination,
                   QGraphicsItem *parent = nullptr);
    ~ConnectionItem() override;

    OutputVertexItem *getSource();
    InputVertexItem *getDestination();

protected:
    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    QPainterPath shape() const override;

private:
    std::array<QPointF, 4> getBezierCurvePoints() const;
    QPointF getSourceShift() const;
    QPointF getDestShift() const;

    OutputVertexItem *_source;
    InputVertexItem *_dest;
};
