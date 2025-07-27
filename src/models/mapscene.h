#ifndef MAPSCENE_H
#define MAPSCENE_H

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

private:
    double m_scale;
    Road *m_buildingRoad = nullptr; // 正在构建的 road
};

#endif // MAPSCENE_H
