#include "node.h"

#include "touristmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

Node::Node(double x, double y, QGraphicsItem *parent)
    : MapItem(parent)
    , m_x(x)
    , m_y(y)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(QGraphicsItem::ItemCoordinateCache);
    setZValue(2);

    QRectF rect(x-radius, y-radius, radius*2, radius*2);
    m_shape.addEllipse(rect);

    m_color = unCheckedColor;
}

void Node::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = checkedColor;
    } else {
        m_color = unCheckedColor;
    }
}

QPainterPath Node::shape() const {
    return m_shape;
}

QRectF Node::boundingRect() const {
    return m_shape.boundingRect();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(m_color);
    painter->setPen(QPen(Qt::black, 1));
    painter->drawPath(m_shape);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    switch (event->button()) {
    case Qt::LeftButton:
        map()->clickNode(this);
        break;
    default:
        break;
    }
}
