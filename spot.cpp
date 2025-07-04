#include "spot.h"

#include "global.h"

#include <QGraphicsSceneMouseEvent>

Spot::Spot(double x, double y, const QString &name, const QString &description, QGraphicsItem *parent)
    : Node(x, y, parent)
    , m_name(name)
    , m_description(description)
{
    setAcceptHoverEvents(true);
    setChecked(false);
}


void Spot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    auto text = QString("%1<br><br>%2").arg(m_name, m_description);
    g_infoTip->showingText(text);
}

void Spot::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    g_infoTip->showAt(event->pos());
}

void Spot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    g_infoTip->hide();
}

void Spot::setChecked(bool checked) {
    checked_ = checked;
    if (checked) {
        color_ = QColor(0, 255, 127);
    } else {
        color_ = QColor(255, 215, 0);
    }
}
