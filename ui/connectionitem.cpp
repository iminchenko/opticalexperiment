#include <QPainter>
#include <math.h>

#include "connectionitem.h"
#include "vertexitem.h"

using std::min;
using std::max;

constexpr double FACTOR = 10;

ConnectionItem::ConnectionItem(OutputVertexItem *source,
                               InputVertexItem *destination,
                               QGraphicsItem *parent)
    :QGraphicsItem(parent), _source(source), _dest(destination) {
    _source->addConnection(this);
    _dest->addConnection(this);
    setPos(0, 0);
    setFlag(ItemIsSelectable);
    setFlag(ItemClipsToShape);
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
    qreal x1 = 9999;
    qreal x2 = -9999;
    qreal y1 = 9999;
    qreal y2 = -9999;

    auto points = getBezierCurvePoints();

    for (size_t i = 0; i < points.size(); ++i) {
        x1 = min(x1, points[i].x());
        y1 = min(y1, points[i].y());
        x2 = max(x2, points[i].x());
        y2 = max(y2, points[i].y());
    }

    return QRectF(QPointF(x1, y1), QPointF(x2, y2));
}

QPainterPath ConnectionItem::shape() const {
    double indent =
           max<double>(abs(_source->scenePos().x() - _dest->scenePos().x()), 1);
    if (indent < abs(_source->scenePos().y() - _dest->scenePos().y())) {
        indent = abs(_source->scenePos().y() - _dest->scenePos().y());
    }
//    indent *= 0.75;

//    indent = std::min<double>(175, indent);

    indent /= FACTOR;

    QPainterPath path;

//    path.addRect(boundingRect());

    auto points = getBezierCurvePoints();

    auto sourceShift = getSourceShift();
    auto destShift = getDestShift();

    auto unit = QLineF(points[0], points[3]).normalVector().unitVector();
    auto centerShift = unit.p2() - unit.p1();
    centerShift = QPointF(centerShift.x() * indent, centerShift.y() * indent);

    path.moveTo(points[0] + sourceShift);

    path.cubicTo(points[1] + sourceShift + centerShift,
                 points[2] + destShift + centerShift,
                 points[3] + destShift);

    path.lineTo(points[3] - destShift);

    path.cubicTo(points[2] - destShift - centerShift,
                 points[1] - sourceShift - centerShift,
                 points[0] - sourceShift);

    return path;
}

void ConnectionItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem *, QWidget *) {
    if (this->isSelected()) {
        painter->setPen(Qt::red);
    } else {
        painter->setPen(Qt::black);
    }
    painter->setBrush(Qt::NoBrush);

    QPainterPath path;

    auto points = getBezierCurvePoints();

    path.moveTo(points[0]);

    path.cubicTo(points[1],  points[2], points[3]);

//    painter->setBrush(Qt::black);
//    path.addPath(shape());

    painter->drawPath(path);
}

std::array<QPointF, 4> ConnectionItem::getBezierCurvePoints() const {
    double indent =
      max<double>(abs(_source->scenePos().x() - _dest->scenePos().x()), 1);
    if (indent < abs(_source->scenePos().y() - _dest->scenePos().y())) {
            indent = abs(_source->scenePos().y() - _dest->scenePos().y());
    }

    indent *= 0.75;

    indent = std::min<double>(175, indent);

    qreal angle = _source->rotation()*M_PI/180;

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

QPointF ConnectionItem::getSourceShift() const {
    auto points = getBezierCurvePoints();

    QLineF line(points[0], points[1]);

    auto normal = line.normalVector().unitVector();

    normal = QLineF(0, 0, normal.dx() * FACTOR, normal.dy() * FACTOR);

    return normal.p2() - normal.p1();
}

QPointF ConnectionItem::getDestShift() const {
    auto points = getBezierCurvePoints();

    QLineF line(points[2], points[3]);

    auto normal = line.normalVector().unitVector();

    normal = QLineF(0, 0, normal.dx() * FACTOR, normal.dy() * FACTOR);

    return normal.p2() - normal.p1();
}
