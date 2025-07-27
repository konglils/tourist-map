#ifndef MAPSCENE_H
#define MAPSCENE_H

#include "infotip.h"
#include "road.h"

#include <QGraphicsScene>

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(QObject *parent = nullptr);

    double scale() { return m_scale; }
    void setScale(double scale) { m_scale = scale; }

    Road *buildingRoad() { return m_buildingRoad; }
    void setBuildingRoad(Road *road) { m_buildingRoad = road; }

    InfoTip *infoTip() { return m_infoTip; }

private:
    double m_scale;
    Road *m_buildingRoad = nullptr; // 正在构建的 road
    InfoTip *m_infoTip = nullptr; // 信息展示框
};

#endif // MAPSCENE_H
