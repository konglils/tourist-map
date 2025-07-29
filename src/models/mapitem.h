#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class TouristMap;

class MapItem : public QGraphicsItem
{
protected:
    MapItem(QGraphicsItem *parent = nullptr);

    TouristMap *map();
};

#endif // MAPITEM_H
