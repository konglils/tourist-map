#ifndef GLOBAL_CPP
#define GLOBAL_CPP

#include "infotip.h"
#include "road.h"
#include "touristmap.h"

enum Mode {
    SelectMode,
    NodeMode,
    SpotMode,
    RoadMode,
    DelMode,
};

inline Mode g_mode = SelectMode; // 模式
inline Road *g_buildingRoad = nullptr; // 正在构建的 road
inline double g_scale; // 地图比例尺
inline InfoTip *g_infoTip = nullptr; // 信息展示框
inline std::unique_ptr<TouristMap> g_map; // 当前打开的地图

#endif // GLOBAL_CPP
