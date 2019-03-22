#pragma once

#include <QGraphicsItem>
#include <list>

class ConnectionItem;
class ConstructorItem;

class VertexItem: public QGraphicsItem {
friend class ConnectionItem;
friend class ConstructorItem;
public:
    VertexItem(ConstructorItem *parent, int number);
    ~VertexItem() override;

    bool canConnect(const VertexItem *vertex);

    int getNumber() const;

    ConnectionItem *getConnection();

protected:
    bool isHoverEvent() const;

    QRectF boundingRect() const override;
    QRectF connectionBoundingRect() const;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    virtual void drawAim(QPainter *painter) const;

    bool addConnection(ConnectionItem *connection);
    void removeConnection();

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

private:
    bool _hoverEvent;

    ConnectionItem* _connection;

    int _number;
};

class InputVertexItem: public VertexItem {
public:
    InputVertexItem(ConstructorItem *parent, int number);

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

class OutputVertexItem: public VertexItem {
public:
    OutputVertexItem(ConstructorItem *parent, int number);
protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};
