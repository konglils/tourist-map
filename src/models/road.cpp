#include "road.h"

#include "infotip.h"
#include "touristmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Road::Road(double startX, double startY, QGraphicsItem *parent)
    : MapItem{parent}
{
    setAcceptHoverEvents(true);
    setZValue(1);

    m_road.moveTo(startX, startY);

    m_color = unCheckedColor;
}

void Road::lineTo(double x, double y) {
    auto last = m_road.elementAt(m_road.elementCount() - 1);
    m_distance += sqrt(pow(x - last.x, 2) + pow(y - last.y, 2));
    m_road.lineTo(x, y);
}

void Road::render() {
    prepareGeometryChange();

    QPainterPathStroker stroker;

    // 线段两端用圆形
    stroker.setCapStyle(Qt::RoundCap);
    // 线段连接处用圆形
    stroker.setJoinStyle(Qt::RoundJoin);

    stroker.setWidth(width + 2);
    m_shape = stroker.createStroke(m_road);

    stroker.setWidth(width - 2);
    m_major = stroker.createStroke(m_road);

    stroker.setWidth(2);
    m_outline = stroker.createStroke(m_major) - m_major;
}

std::vector<std::pair<double, double>> Road::points() const {
    std::vector<std::pair<double, double>> allPoints;
    allPoints.reserve(m_road.elementCount());
    for (int i = 0; i < m_road.elementCount(); i += 1) {
        auto element = m_road.elementAt(i);
        allPoints.emplace_back(element.x, element.y);
    }
    return allPoints;
}

void Road::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = checkedColor;
    } else {
        m_color = unCheckedColor;
    }
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->fillPath(m_major, m_color);
    painter->fillPath(m_outline, Qt::black);
}

void Road::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    switch (event->button()) {
    case Qt::LeftButton:
        map()->clickRoad(this);
        break;
    default:
        break;
    }
}

void Road::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    auto text = QString("%1 m").arg((int)round(m_distance * map()->scale()));
    map()->infoTip()->showText(text);
}

void Road::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    map()->infoTip()->moveTo(mapToScene(event->pos()));
}

void Road::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    map()->infoTip()->hide();
}
