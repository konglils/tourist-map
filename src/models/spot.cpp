#include "mapscene.h"
#include "spot.h"

#include <QGraphicsSceneMouseEvent>

Spot::Spot(double x, double y, const QString &name, const QString &description, QGraphicsItem *parent)
    : Node(x, y, parent)
    , m_name(name)
    , m_description(description)
{
    setAcceptHoverEvents(true);
    m_color = QColor(255, 215, 0);
}

void Spot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    auto text = QString("%1<br><br>%2").arg(m_name, m_description);
    mapScene()->infoTip()->showingText(text);
}

void Spot::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    mapScene()->infoTip()->showAt(event->pos());
}

void Spot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    mapScene()->infoTip()->hide();
}

void Spot::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = QColor(0, 255, 127);
    } else {
        m_color = QColor(255, 215, 0);
    }
}
