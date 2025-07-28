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

    /**
     * @brief 显示指定地图
     * @param map 地图
     */
    void showMap(TouristMap *map);

    /**
     * @brief 在视口中心缩放
     * @param zoomIn 是否放大
     */
    void zoom(bool zoomIn);

    /**
     * @brief 打开默认地图
     */
    void open();

    /**
     * @brief 打开地图文件
     * @param filePath 文件路径
     */
    void openFile(const QString &filePath);

    /**
     * @brief 保存当前地图
     */
    void save();

    /**
     * @brief 创建新地图
     * @param imageFilePath 参考图路径
     * @param mapTitle 地图标题
     * @param mapScale 地图比例尺
     */
    void createMap(const QString &imageFilePath,
                   const QString &mapTitle,
                   double mapScale);

    void setMode(Mode mode);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    /// 在视口指定位置缩放
    void zoom(bool zoomIn, QPointF pos);

    void setTitle(const QString &title);

    static constexpr double zoomFactor = 1.2;

    bool m_dragging = false; ///< 鼠标是否正在拖拽
    QPointF m_mousePos;

    TouristMap *m_map = nullptr;
    MapScene *m_scene = nullptr;
};

#endif // MAPVIEW_H
