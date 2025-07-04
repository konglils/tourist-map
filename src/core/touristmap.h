#ifndef TOURISTMAP_H
#define TOURISTMAP_H

/* 文件格式
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

#include "road.h"

#include <QGraphicsScene>
#include <QString>

class MapView;

class TouristMap
{
public:
    TouristMap(MapView *view);
    QGraphicsScene *scene() { return m_scene; }
    bool openFile(const QString &fileName); // 打开文件
    bool saveFile(const QString &fileName); // 保存文件
    bool save(); // 保存
    QString name() const { return m_name; }
    double scale() const { return m_mapScale; }
    bool setImage(const QString &imageFileName); // 设置参考图
    void setName(const QString &name) { m_name = name;}
    void setScale(double scale) { m_mapScale = scale; }
    void clear(); // 清除地图选中状态
    void addNode(Node *node); // 添加结点
    void addRoad(Road *road); // 添加道路
    void delNode(Node *node); // 删除结点
    void delRoad(Road *road); // 删除道路
    void pressNode(Node *node); // 按下节点

private:
    QString m_fileName; // 文件名
    QByteArray m_image; // 参考图原始数据
    double m_mapScale; // 地图比例尺 meter : pixel
    QString m_name; // 地图名称
    QGraphicsScene *m_scene; // 对应的 scene
    std::vector<std::vector<std::pair<Node *, Road *>>> m_graph; // 图
    Node *m_source = nullptr; // 初始未设置源点
    Node *m_destination = nullptr; // 初始未设置目的地
    std::vector<double> m_shortestDistance; // 最短距离
    std::vector<std::pair<Node *, Road *>> m_shortestPath; // 最短路径
    std::vector<Node *> m_nodes; // 用于确定 node 对应的索引

    bool loadImage(); // 加载图片到 scene
    bool readImage(QDataStream &in); // 读取图片
    void calShortestPath(Node *source); // 计算最短路
    void setPathShow(Node *destination, bool show); // 设置路径是否可见
    bool readNode(QDataStream &in, quint64 startIndex, quint64 numNode); // 读取路口
    bool readSpot(QDataStream &in, quint64 startIndex, quint64 numSpot); // 读取地点
    bool readRoad(QDataStream &in, quint64 numRoad); // 读取道路
    std::tuple<quint64, quint64, quint64> reCalItems(); // 重新计算图中元素数量并更新 node 的索引
    bool writePicture(QDataStream &out); // 写入图片
    bool writeNode(QDataStream &out, quint64 startIndex, quint64 numNode); // 写入路口
    bool writeSpot(QDataStream &out, quint64 startIndex, quint64 numSpot); // 写入地点
    bool writeRoad(QDataStream &out); // 写入道路
};

#endif // TOURISTMAP_H
