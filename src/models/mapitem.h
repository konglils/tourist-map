#ifndef MAPITEM_H
#define MAPITEM_H

#include "mapscene.h"

#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
protected:
    MapItem(QGraphicsItem *parent = nullptr);

    MapScene *mapScene() { return static_cast<MapScene *>(scene()); }

    TouristMap *map() { return mapScene()->map(); }
};

#endif // MAPITEM_H
