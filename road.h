#ifndef ROAD_H
#define ROAD_H

#include "node.h"

#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QLabel>

class Road : public QGraphicsItem
{
public:
    Road(double x, double y, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override { return m_shape.boundingRect(); }
    QPainterPath shape() const override { return m_shape; }
    void render(); // 渲染当前已连线的点，变成一条带状折线
    void lineTo(double x, double y); // 从当前点连接到新点
    bool isChecked() const { return m_checked; };
    void setChecked(bool checked);
    double distance() { return m_distance; }
    Node *node1() { return m_node1; }
    Node *node2() { return m_node2; }
    void setNode1(Node *node) { m_node1 = node; }
    void setNode2(Node *node) { m_node2 = node; }
    std::vector<std::pair<double, double>> points() const; // 返回经过的点的坐标

    enum { Type = QGraphicsItem::UserType + 3 };
    int type() const override { return Type; }

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    static constexpr int m_WIDTH = 10; // 道路显示宽度
    Node *m_node1 = nullptr; // 连接的第1个 node
    Node *m_node2 = nullptr; // 连接的第2个 node
    QPainterPath m_road; // 以折线段表示的路
    double m_distance = 0.; // 道路长度
    QPainterPath m_shape; // 路的实际形状，用于绘图
    QPainterPath m_major; // 路的主体
    QPainterPath m_outline; // 路的轮廓
    // QColor m_color = QColor(255, 255, 255, 100); // 路的颜色
    QColor m_color = Qt::blue;
    bool m_checked = false; // 是否被选中
};

#endif // ROAD_H
