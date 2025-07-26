#include "node.h"

#include "global.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Node::Node(double x, double y, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_x(x)
    , m_y(y)
{
    setZValue(2);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(QGraphicsItem::ItemCoordinateCache);

    QRectF rect(x-m_RADIUS, y-m_RADIUS, m_RADIUS*2, m_RADIUS*2);
    m_shape.addEllipse(rect);
}

QRectF Node::boundingRect() const {
    return m_shape.boundingRect();
}

QPainterPath Node::shape() const {
    return m_shape;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(m_color);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(m_shape);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (g_mode == SelectMode) {
            g_map->pressNode(this);
        } else if (g_mode == RoadMode) {
            // 路只能以节点开始和结束
            if (g_buildingRoad) {
                g_buildingRoad->lineTo(m_x, m_y);
                g_buildingRoad->render();
                g_buildingRoad->update();
                g_buildingRoad->setNode2(this);
                g_map->addRoad(g_buildingRoad);
                g_buildingRoad = nullptr;
            } else {
                g_buildingRoad = new Road(m_x, m_y);
                g_buildingRoad->setNode1(this);
                scene()->addItem(g_buildingRoad);
            }
        } else if (g_mode == DelMode) {
            g_map->delNode(this);
        }
    }
}

void Node::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = Qt::black;
    } else {
        m_color = QColor(255, 255, 255, 100);
    }
}
