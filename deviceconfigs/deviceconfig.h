#pragma once

#include <string>
#include <vector>
#include <list>

#include <QPainter>
#include <QRectF>
#include <QDebug>

#include "globaldefines.h"
#include "matrix.h"
#include "devicedefines.h"

struct DrawingConfig {
    enum drawingType {
        TYPE_SIZE = 0,
        TYPE_LINE,
        TYPE_RECT,
        TYPE_CIRCLE,
        TYPE_TEXT,
        TYPE_UNKNOWN
    };

    // возможно стоит отсюда перенести
    static drawingType toType(const std::string &type) {
        if (type == "size")
            return TYPE_SIZE;
        if (type == "line")
            return TYPE_LINE;
        if (type == "rect")
            return TYPE_RECT;
        if (type == "circle")
            return TYPE_CIRCLE;
        if (type == "text")
            return TYPE_TEXT;

#ifdef _DEBUG
        qDebug() << "Unknown string drawing type" << type.c_str();
#endif

        return TYPE_UNKNOWN;
    }

    std::string text;
    drawingType type;
    std::vector<int> coordinates;
};

class DeviceConfig {
public:
    DeviceConfig() = default;
    DeviceConfig(int inputCount, int outputCount, const std::string &name,
        const std::string &descrption, const std::list<DrawingConfig> &drawing,
                                       const TransMatrix &matr);

    void setSize(float width, float height);
    void setBounding(const QRectF& rect);

    QRectF getBounding() const;

    std::string getName() const;
    std::string getDescription() const;

    int getInputCount() const;
    int getOutputCount() const;

    const TransMatrix &getMatrix() const;

    void draw(QPainter *painter, bool selected = false) const;

private:
    int _inputs = 0;
    int _outputs = 0;

    TransMatrix _matrix;

    std::string _name;
    std::string _description;

    QRectF _bounding;

    std::list<DrawingConfig> _drawings;
};
