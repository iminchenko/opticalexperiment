#include "deviceviewconfig.h"

DeviceViewConfig::DeviceViewConfig(int inputCount, int outputCount,
    const std::string &name, const std::string &descrption,
                                   const std::list<DrawingConfig> &drawing)
    :_inputs(inputCount), _outputs(outputCount), _name(name),
                                _description(descrption), _drawings(drawing) {
    auto iter = _drawings.begin();

    while (iter != _drawings.end()) {
        switch (iter->type) {
        case DrawingConfig::TYPE_SIZE:
            setSize(iter->coordinates[0], iter->coordinates[1]);
            iter = _drawings.erase(iter);
            break;

        default:
            ++iter;
            break;
        }
    }
}

void DeviceViewConfig::setSize(float width, float height) {
    setBounding(QRectF(-width/2, -height/2, width, height));
}

void DeviceViewConfig::setBounding(const QRectF &rect) {
    _bounding = rect;
}

QRectF DeviceViewConfig::getBounding() const {
    return _bounding;
}

std::string DeviceViewConfig::getName() const {
    return _name;
}

std::string DeviceViewConfig::getDescription() const {
    return _description;
}

int DeviceViewConfig::getInputCount() const {
    return _inputs;
}

int DeviceViewConfig::getOutputCount() const {
    return _outputs;
}

void DeviceViewConfig::draw(QPainter *painter, bool selected) const {
    painter->setPen(QPen(Qt::black, 2));

    if (!selected)
        painter->setBrush(QColor(200, 225, 255));
    else
        painter->setBrush(QColor(200, 255, 225));

    float halfWidth = _bounding.width() / 2;
    float halfHeight = _bounding.height() / 2;
    painter->drawRoundRect(QRectF(-halfWidth, -halfHeight,
                                  _bounding.width(), _bounding.height()));

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
        case DrawingConfig::TYPE_TEXT:
            painter->drawText(_bounding, Qt::AlignCenter, iter.text.c_str());
            break;
        default:
            break;
        }
    }
}
