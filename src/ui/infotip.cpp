#include "infotip.h"

InfoTip::InfoTip(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setZValue(20);
}

void InfoTip::showText(const QString &text) {
    auto tip = QString(
        "<div style=\""
        "background-color: white;"
        "color: black;"
        "font-size: 20px\">"
        "%1"
        "</div>"
        ).arg(text);
    setHtml(tip);
    show();
}

void InfoTip::moveTo(const QPointF &pos) {
    setPos(pos + QPoint(15, 15));
}
