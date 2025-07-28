#include "road.h"

#include "infotip.h"
#include "mapscene.h"
#include "touristmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Road::Road(double x, double y, QGraphicsItem *parent)
    : MapItem(parent)
{
    setAcceptHoverEvents(true);
    setZValue(1);
    m_road.moveTo(x, y);
}

void Road::render() {
    prepareGeometryChange();
    QPainterPathStroker stroker;
    stroker.setCapStyle(Qt::RoundCap); // 线段两端用圆形
    stroker.setJoinStyle(Qt::RoundJoin); // 线段连接处用圆形

    stroker.setWidth(m_WIDTH + 2);
    m_shape = stroker.createStroke(m_road);

    stroker.setWidth(m_WIDTH - 2);
    m_major = stroker.createStroke(m_road);

    stroker.setWidth(2);
    m_outline = stroker.createStroke(m_major) - m_major;
}

void Road::lineTo(double x, double y) {
    auto last = m_road.elementAt(m_road.elementCount() - 1);
    m_distance += sqrt(pow(x - last.x, 2) + pow(y - last.y, 2));
    m_road.lineTo(x, y);
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->fillPath(m_major, m_color);
    painter->fillPath(m_outline, Qt::black);
}

void Road::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        map()->clickRoad(this);
    }
}

void Road::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    auto text = QString("%1 m").arg((int)round(m_distance * map()->scale()));
    mapScene()->infoTip()->showingText(text);
}

void Road::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    mapScene()->infoTip()->showAt(mapToScene(event->pos()));
}

void Road::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    mapScene()->infoTip()->hide();
}

void Road::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = QColor("#4169E1");
    } else {
        m_color = QColor(255, 255, 255, 100);
    }
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
