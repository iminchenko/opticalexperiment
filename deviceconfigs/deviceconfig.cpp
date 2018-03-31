#include "deviceconfig.h"

DeviceConfig::DeviceConfig(int inputCount, int outputCount,
                        const std::string &name, const std::string &descrption,
                        const std::list<DrawingConfig> &drawing,
                        const TransMatrix &matr)
    :_inputs(inputCount), _outputs(outputCount), _matrix(matr), _name(name),
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

void DeviceConfig::setSize(float width, float height) {
    setBounding(QRectF(-width/2, -height/2, width, height));
}

void DeviceConfig::setBounding(const QRectF &rect) {
    _bounding = rect;
}

QRectF DeviceConfig::getBounding() const {
    return _bounding;
}

std::string DeviceConfig::getName() const {
    return _name;
}

std::string DeviceConfig::getDescription() const {
    return _description;
}

int DeviceConfig::getInputCount() const {
    return _inputs;
}

int DeviceConfig::getOutputCount() const {
    return _outputs;
}

void DeviceConfig::draw(QPainter *painter, bool selected) const {
    painter->setPen(QPen(Qt::black, 2));

    if (!selected)
        painter->setBrush(QColor(200, 225, 255));
    else
        painter->setBrush(QColor(200, 255, 225));

    qreal halfWidth = _bounding.width() / 2;
    qreal halfHeight = _bounding.height() / 2;
    painter->drawRoundRect(QRectF(-halfWidth, -halfHeight,
                                  _bounding.width(), _bounding.height()));

    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(Qt::NoBrush);

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

const TransMatrix &DeviceConfig::getMatrix() const {
    return _matrix;
}
