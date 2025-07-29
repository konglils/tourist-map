#include "mapitem.h"

#include "touristmap.h"

MapItem::MapItem(QGraphicsItem *parent)
    : QGraphicsItem{parent}
{}

TouristMap *MapItem::map() {
    return static_cast<TouristMap *>(scene());
}
