#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "infotip.h"

#include <QGraphicsScene>

class TouristMap;

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(TouristMap *map, QObject *parent = nullptr);

    InfoTip *infoTip() { return m_infoTip; }

    TouristMap *map() { return m_map; }

private:
    InfoTip *m_infoTip = nullptr; // 信息展示框
    TouristMap *m_map;
};

#endif // MAPSCENE_H
