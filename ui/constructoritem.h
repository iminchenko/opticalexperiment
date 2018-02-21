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

    ConstructorItem(QPointF pos, QGraphicsItem *parent = nullptr);

    virtual ~ConstructorItem() = 0;

    virtual int getItemType() const;
    virtual bool isSource() const;
    virtual bool isReciever() const;

    QColor getBackgroundColor() const;
    QColor getSelectedColor() const;
    QColor getBorderColor() const;
    float getBorderWidth() const;
    float getWidth() const;

protected:
    virtual QRectF boundingRect() const override;

    virtual void keyPressEvent(QKeyEvent *event) override;

    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    void drawBox(QPainter *painter);

    void addOutputVertex(float angle = 0);
    void addInputVertex(float angle = 0);

    static const QColor defaultBackgroundColor;
    static const QColor defaultSelectedColor;
    static const QColor defaultBorderColor;
    static const float defaultBorderWidth;
    static const float defaultWidth;

private:
    QColor _backgroundColor;
    QColor _selectedColor;
    QColor _borderColor;
    float _borderWidth;
    float _width;

    bool _source;
    bool _reciever;
};
