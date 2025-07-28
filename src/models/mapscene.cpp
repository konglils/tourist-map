#include "mapscene.h"

MapScene::MapScene(TouristMap *map, QObject *parent)
    : QGraphicsScene{parent}
    , m_infoTip{new InfoTip}
    , m_map{map}
{
    addItem(m_infoTip);
}
