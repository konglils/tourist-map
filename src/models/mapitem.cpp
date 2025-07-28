#include "mapitem.h"

#include "mapscene.h"

MapItem::MapItem(QGraphicsItem *parent)
    : QGraphicsItem{parent}
{}

MapScene *MapItem::mapScene() {
    return static_cast<MapScene *>(scene());
}

TouristMap *MapItem::map() {
    return mapScene()->map();
}
