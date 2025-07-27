#ifndef GLOBAL_CPP
#define GLOBAL_CPP

#include "touristmap.h"

enum Mode {
    SelectMode,
    NodeMode,
    SpotMode,
    RoadMode,
    DelMode,
};

inline Mode g_mode = SelectMode; // 模式
inline std::unique_ptr<TouristMap> g_map; // 当前打开的地图

#endif // GLOBAL_CPP
