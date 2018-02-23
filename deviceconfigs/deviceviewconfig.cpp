#include "deviceviewconfig.h"

DeviceViewConfig::DeviceViewConfig(const std::string &name,
                                   const std::string &descrption, const std::vector<DrawingConfig> &drawing)
    :_name(name), _description(descrption), _drawings(drawing) {}

std::string DeviceViewConfig::getName() const {
    return _name;
}

std::string DeviceViewConfig::getDescription() const {
    return _description;
}

void DeviceViewConfig::draw(QPainter *painter) const {
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));

    for (const auto &iter : _drawings) {
        // кажется в этом свитче многовато дублирований
        switch (iter.type) {
        case DrawingConfig::TYPE_CIRCLE:
            painter->drawEllipse(iter.coordinates[0], iter.coordinates[1],
                                 iter.coordinates[2], iter.coordinates[3]);
            break;
        case DrawingConfig::TYPE_LINE:
            painter->drawLine(iter.coordinates[0], iter.coordinates[1],
                              iter.coordinates[2], iter.coordinates[3]);
            break;
        case DrawingConfig::TYPE_RECT:
            painter->drawRect(iter.coordinates[0], iter.coordinates[1],
                              iter.coordinates[2], iter.coordinates[3]);
            break;
        case DrawingConfig::TYPE_SIZE:
            // ????????
            break;
        case DrawingConfig::TYPE_TEXT:
            painter->drawText(QPointF(-10, 5), iter.text.c_str());
            break;
        }
    }
}
