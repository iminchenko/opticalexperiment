#pragma once

#include <string>
#include <vector>

#include <QPainter>

struct DrawingConfig {
    enum drawingType {
        TYPE_SIZE = 0,
        TYPE_LINE,
        TYPE_RECT,
        TYPE_CIRCLE,
        TYPE_TEXT
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
    }

    std::string text;
    drawingType type;
    std::vector<int> coordinates;
};

class DeviceViewConfig {
public:
    DeviceViewConfig() = default;
    DeviceViewConfig(const std::string &name, const std::string &descrption,
                                    const std::vector<DrawingConfig> &drawing);

    std::string getName() const;
    std::string getDescription() const;

    void draw(QPainter *painter) const;

private:
    std::string _name;
    std::string _description;

    std::vector<DrawingConfig> _drawing;
};
