#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "mode.h"

#include <QGraphicsView>
#include <QPointF>
#include <QString>

class MapScene;
class QMouseEvent;
class QWheelEvent;
class QWidget;
class SpotEditor;
class TouristMap;

/**
 * @brief 显示场景和界面交互
 */
class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent = nullptr);

    /// 在视口中心放大
    void zoomIn();

    /// 在视口中心缩小
    void zoomOut();

    /// 选择地图文件打开地图
    void open();

    /// 打开默认地图
    void openDefault();

    /// 保存当前地图
    void save();

    /// 创建新地图
    void newMap();

    /// 设置地图的操作模式
    void setMode(int mode);

signals:
    /**
     * @brief 模式改变的信号
     * @param mode 改变后的模式；-1 表示未打开地图
     */
    void modeChanged(int mode);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void openFile(const QString &filePath);

    TouristMap *map() const;

    void setMap(TouristMap *map);

    /// 在视口指定位置缩放
    void zoom(bool zoomIn, QPointF pos);

    void setTitle(const QString &title);

    static constexpr double zoomFactor = 1.2;

    bool m_dragging = false; ///< 鼠标是否正在拖拽
    QPointF m_mousePos;
};

#endif // MAPVIEW_H
