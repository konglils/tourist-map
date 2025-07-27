#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "touristmap.h"

#include <QGraphicsView>

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView(QWidget *parent = nullptr);
    void open(); // 打开地图
    void openFile(const QString &fileName); // 打开地图文件
    void save(); // 保存地图
    void enlarge(bool flag); // 放大或缩小
    void setTitle(const QString &name); // 设置窗口标题
    void createMap(const QString &imageFileName, const QString &mapName, double mapScale); // 创建新地图
    void setNewMap(std::unique_ptr<TouristMap> map); // 设置显示哪个地图

protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override; // 重写鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override; // 重写鼠标释放事件
    void wheelEvent(QWheelEvent *event) override; // 重写鼠标滚轮事件

private:
    static constexpr double m_SCALING = 1.2; // 放大缩小的比例
    bool m_dragging = false; // 鼠标是否正在拖拽
    QPointF m_mousePos; // 鼠标位置，用于编写拖拽逻辑
    void enlarge(bool flag, QPointF mousePos); // 指定位置处缩放

    void inputInfo(QPointF point); // 在指定位置创建输入框输入信息
};

#endif // MAPVIEW_H
