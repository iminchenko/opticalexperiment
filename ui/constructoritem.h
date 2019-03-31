#pragma once

#include <QGraphicsItem>
#include <QPainter>

#include "properties.h"
#include "vertexitem.h"

class ConstructorItem : public QGraphicsItem, public Properties {
public:
    enum itemType {
        TYPE_SOURCE = 1,
        TYPE_RECIEVER = 2,
    };

    ConstructorItem(QPointF pos, int id, QGraphicsItem *parent = nullptr);

    ~ConstructorItem() override = 0;

    virtual int getItemType() const;
    virtual bool isSource() const;
    virtual bool isReciever() const;

    QColor getBackgroundColor() const;
    QColor getSelectedColor() const;
    QColor getBorderColor() const;
    float getBorderWidth() const;
    float getWidth() const;

    int getId() const override;

    OutputVertexItem *getOutput(int number);
    InputVertexItem *getInput(int number);

protected:
    QRectF boundingRect() const override;

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    void drawBox(QPainter *painter) const;

    void addOutputVertex(float angle = 0);
    void addInputVertex(float angle = 0);

    static const QColor DEFAULT_BACKGROUND_COLOR;
    static const QColor DEFAULT_SELECTED_COLOR;
    static const QColor DEFAULT_BORDER_COLOR;
    static const float DEFAULT_BORDER_WIDTH;
    static const float DEFAULT_WIDTH;

private:
    QColor _backgroundColor;
    QColor _selectedColor;
    QColor _borderColor;
    float _borderWidth;
    float _width;

    bool _source;
    bool _reciever;

    int _outCount;
    int _inCount;
    int _id;

    QList<InputVertexItem*> _inputs;
    QList<OutputVertexItem*> _outputs;
};
