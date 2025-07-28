#include "node.h"

#include "touristmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Node::Node(double x, double y, QGraphicsItem *parent)
    : MapItem(parent)
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
        map()->pressNode(this);
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
