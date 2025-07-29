#ifndef TOURISTMAP_H
#define TOURISTMAP_H

#include "mode.h"

#include <QPointF>
#include <QString>

class MapScene;
class Node;
class Road;

/**
 * @brief 打开或保存地图文件；修改地图内容；计算最短路径
 *
 * 一个 TouristMap 对应一个场景（MapScene），读取文件产生的图形保存在场景里，
 * MapView 可以显示该场景。
 *
 * 节点：包括路口和地点，抽象出来方便图的计算
 *
 * 路口：拥有坐标
 *
 * 地点：拥有坐标、名称和描述
 *
 * 道路：双行道。保存连接的两个节点。可以是折线，保存折线上所有点的坐标
 *
 * 地图文件格式
 * 大端序
 * 12 字节 TOURIST MAP\0
 *  4 字节 0x52391391
 *  8 字节 u64 图片大小
 *  8 字节 f64 比例尺
 *  8 字节 u64 路口数
 *  8 字节 u64 地点数
 *  8 字节 u64 道路数
 *    变长 字符串 标题
 *    变长 路口
 *    变长 地点
 *    变长 道路
 *    变长 图片
 *
 * 字符串
 *  8 字节 u64 utf-8 编码长度
 *    变长 字节序列
 *
 * 坐标
 *  8 字节 f64 x
 *  8 字节 f64 y
 *
 * 路口
 * 16 字节 坐标
 *
 * 地点
 * 16 字节 坐标
 *    变长 字符串 名称
 *    变长 字符串 描述
 *
 * 道路
 *  8 字节 u64 节点1索引
 *  8 字节 u64 节点2索引
 *  8 字节 u64 途经点数
 *    变长 坐标 途经点
 */
class TouristMap
{
public:
    TouristMap();

    ~TouristMap();

    /**
     * @brief 打开地图文件，将二进制数据转换为图形
     * @param filePath 文件路径
     * @return 打开成功或失败
     */
    bool openFile(const QString &filePath);

    /**
     * @brief 将当前地图图形转换为二进制数据，保存为地图文件
     * @param filePath 文件路径
     * @return 保存成功或失败
     */
    bool saveFile(const QString &filePath);

    /**
     * @brief 保存当前地图
     * @return 保存成功或失败
     */
    bool save();

    /**
     * @brief 点击节点
     * @param node 节点
     */
    void clickNode(Node *node);

    /**
     * @brief 点击道路
     * @param road 道路
     */
    void clickRoad(Road *road);

    /**
     * @brief 点击了背景
     * @param pos 点击的场景坐标
     */
    void clickBackground(QPointF pos);

    /**
     * @brief 清除地图选中状态
     */
    void clear();

    /**
     * @brief 向当前地图添加节点
     * @param node 节点
     */
    void addNode(Node *node);

    /**
     * @brief 向当前地图添加道路
     * @param road 道路
     */
    void addRoad(Road *road);

    /**
     * @brief 从当前地图删除节点
     * @param node 节点
     */
    void delNode(Node *node);

    /**
     * @brief 从当前地图删除道路
     * @param road 道路
     */
    void delRoad(Road *road);

    /**
     * @brief 为当前地图设置参考图
     * @param 图片文件路径
     * @return 是否成功
     */
    bool setImage(const QString &imageFilePath);

    MapScene *scene() const { return m_scene; }

    void setMode(Mode mode) { m_mode = mode; }
    Mode mode() const { return m_mode; }

    void setTitle(const QString &title) { m_title = title;}
    QString title() const { return m_title; }

    void setScale(double scale) { m_scale = scale; }
    double scale() const { return m_scale; }

    Road *buildingRoad() const { return m_buildingRoad; }

private:
    bool readImage(QDataStream &in);
    bool loadImage();
    bool readNode(QDataStream &in, quint64 startIndex, quint64 numNode);
    bool readSpot(QDataStream &in, quint64 startIndex, quint64 numSpot);
    bool readRoad(QDataStream &in, quint64 numRoad);
    bool writeImage(QDataStream &out);
    bool writeNode(QDataStream &out, quint64 startIndex, quint64 numNode);
    bool writeSpot(QDataStream &out, quint64 startIndex, quint64 numSpot);
    bool writeRoad(QDataStream &out);

    /// 重新计算图中元素数量并更新节点的索引
    std::tuple<quint64, quint64, quint64> reCalItems();

    void calShortestPath(Node *source);
    void setPathShow(Node *destination, bool show);

    MapScene *m_scene;

    Mode m_mode;

    QString m_title;
    double m_scale;

    QString m_filePath;
    QByteArray m_image; ///< 参考图原始数据

    /// 每个节点都对应了一个索引，保存索引对应的节点
    std::vector<Node *> m_nodes;

    std::vector<std::vector<std::pair<Node *, Road *>>> m_graph; ///< 图
    Node *m_source = nullptr;
    Node *m_destination = nullptr;
    std::vector<double> m_shortestDistance;
    /// 保存节点在最短路树中的父节点和连接的道路，这样就可以一路回溯到源点
    std::vector<std::pair<Node *, Road *>> m_shortestPath;

    Road *m_buildingRoad = nullptr;
};

#endif // TOURISTMAP_H
