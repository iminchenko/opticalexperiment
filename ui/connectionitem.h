#pragma once

#include <QGraphicsItem>
#include <array>

class VertexItem;
class InputVertexItem;
class OutputVertexItem;

class ConnectionItem : public QGraphicsItem {
friend class VertexItem;
public:
    ConnectionItem(OutputVertexItem *source, InputVertexItem *destination,
                   QGraphicsItem *parent = nullptr);
    ~ConnectionItem();

    OutputVertexItem *getSource();
    InputVertexItem *getDestination();

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    std::array<QPointF, 4> getBezierCurvePoints() const;

    OutputVertexItem *_source;
    InputVertexItem *_dest;
};
