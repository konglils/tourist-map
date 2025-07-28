#ifndef MAPITEM_H
#define MAPITEM_H

#include "mapscene.h"

#include <QGraphicsItem>

class MapItem : public QGraphicsItem
{
protected:
    MapItem(QGraphicsItem *parent = nullptr);

    MapScene *mapScene();

    TouristMap *map();
};

#endif // MAPITEM_H
