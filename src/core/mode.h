#ifndef MODE_H
#define MODE_H

/**
 * @brief 定义地图的操作模式
 */
enum Mode {
    /// 选择节点和道路，查看最短路径
    SelectMode = 0,

    /// 添加路口
    NodeMode,

    /// 添加地点
    SpotMode,

    /// 添加道路
    RoadMode,

    /// 删除节点或道路
    DelMode,
};

#endif // MODE_H
