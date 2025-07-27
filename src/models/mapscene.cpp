#include "mapscene.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene{parent}
    , m_infoTip{new InfoTip}
{
    addItem(m_infoTip);
}
