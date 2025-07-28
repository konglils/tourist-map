#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class MapScene;
class TouristMap;

class MapItem : public QGraphicsItem
{
protected:
    MapItem(QGraphicsItem *parent = nullptr);

    MapScene *mapScene();

    TouristMap *map();
};

#endif // MAPITEM_H
