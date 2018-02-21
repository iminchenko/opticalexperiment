#include <QPainter>
#include <math.h>

#include "connectionitem.h"
#include "vertexitem.h"

using std::min;
using std::max;

ConnectionItem::ConnectionItem(OutputVertexItem *source,
                               InputVertexItem *destination, QGraphicsItem *parent)
    :QGraphicsItem(parent), _source(source), _dest(destination) {
    _source->addConnection(this);
    _dest->addConnection(this);
    setPos(0, 0);
}

ConnectionItem::~ConnectionItem() {
    _source->removeConnection();
    _dest->removeConnection();
}

OutputVertexItem *ConnectionItem::getSource() {
    return _source;
}

InputVertexItem *ConnectionItem::getDestination() {
    return _dest;
}

QRectF ConnectionItem::boundingRect() const {
    qreal x1, x2, y1, y2;

    auto points = getBezierCurvePoints();

    for (size_t i = 0; i < points.size(); ++i) {
        x1 = min(x1, points[i].x());
        y1 = min(y1, points[i].y());
        x2 = max(x2, points[i].x());
        y2 = max(y2, points[i].y());
    }

    return QRectF(QPointF(x1, y1), QPointF(x2, y2));
}

void ConnectionItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);

    QPainterPath path;

    auto points = getBezierCurvePoints();

    path.moveTo(points[0]);

    path.cubicTo(points[1],  points[2], points[3]);

    painter->drawPath(path);
}

std::array<QPointF, 4> ConnectionItem::getBezierCurvePoints() const {
    double indent =
      max<double>(abs(_source->scenePos().x() - _dest->scenePos().x()), 1);
    if (indent < abs(_source->scenePos().y() - _dest->scenePos().y()))
            indent = abs(_source->scenePos().y() - _dest->scenePos().y());
    indent *= 0.75;

    indent = std::min<double>(175, indent);

    float angle = _source->rotation()*M_PI/180;

    std::array<QPointF, 4> points;

    points[0] = _source->scenePos();
    points[1] = QPointF(points[0].x() + indent * cos(angle),
                        points[0].y() + indent * sin(angle));

    angle = _dest->rotation()*M_PI/180;
    points[3] = _dest->scenePos();
    points[2] = QPointF(points[3].x() - indent * cos(angle),
                                points[3].y() - indent * sin(angle));

    return points;
}
