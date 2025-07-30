#ifndef ROAD_H
#define ROAD_H

#include "mapitem.h"

#include <QColor>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QRectF>

class Node;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;

/**
 * @brief 道路类，设定为双行道
 */
class Road : public MapItem
{
public:
    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const override { return Type; }

    Road(double startX, double startY, QGraphicsItem *parent = nullptr);

    /// 从当前点连接到新点
    void lineTo(double x, double y);

    /// 渲染当前已连线的点，变成一条有宽度的线
    void render();

    /// 返回经过的点的坐标
    std::vector<std::pair<double, double>> points() const;

    void setChecked(bool checked);
    bool isChecked() const { return m_checked; };

    void setNode1(Node *node) { m_node1 = node; }
    Node *node1() const { return m_node1; }

    void setNode2(Node *node) { m_node2 = node; }
    Node *node2() const { return m_node2; }

    double distance() const { return m_distance; }

    QPainterPath shape() const override { return m_shape; }

    QRectF boundingRect() const override { return m_shape.boundingRect(); }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    static constexpr int width = 10; ///< 道路显示宽度

    static constexpr QColor unCheckedColor = QColor(255, 255, 255, 100);
    static constexpr QColor checkedColor = QColor(65, 105, 225);

    QPainterPath m_road; ///< 以折线段表示的路

    Node *m_node1 = nullptr;
    Node *m_node2 = nullptr;

    bool m_checked = false;

    double m_distance = 0.;

    QPainterPath m_shape; ///< 路的实际形状，用于绘图
    QPainterPath m_major;
    QPainterPath m_outline;

    QColor m_color;
};

#endif // ROAD_H
