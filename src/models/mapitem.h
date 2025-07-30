#ifndef MAPITEM_H
#define MAPITEM_H

#include <QGraphicsItem>

class TouristMap;

/**
 * @brief 地图元素的基类
 */
class MapItem : public QGraphicsItem
{
protected:
    MapItem(QGraphicsItem *parent = nullptr);

    /// 返回元素所在的地图
    TouristMap *map();
};

#endif // MAPITEM_H
