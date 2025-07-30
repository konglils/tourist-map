#include "spot.h"

#include "infotip.h"
#include "touristmap.h"

#include <QGraphicsSceneHoverEvent>

Spot::Spot(double x, double y,
           const QString &name, const QString &description,
           QGraphicsItem *parent)
    : Node(x, y, parent)
    , m_name(name)
    , m_description(description)
{
    setAcceptHoverEvents(true);

    m_color = unCheckedColor;
}

void Spot::setChecked(bool checked) {
    m_checked = checked;
    if (checked) {
        m_color = checkedColor;
    } else {
        m_color = unCheckedColor;
    }
}

void Spot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    auto text = QString("%1<br><br>%2").arg(m_name, m_description);
    map()->infoTip()->showingText(text);
}

void Spot::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    map()->infoTip()->showAt(event->pos());
}

void Spot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    map()->infoTip()->hide();
}
