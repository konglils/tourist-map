#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>

class Node : public QGraphicsItem
{
public:
    Node(double x, double y, QGraphicsItem *parent = nullptr); // 提供坐标来初始化
    QRectF boundingRect() const override; // 重写图形更新区域（矩形）
    QPainterPath shape() const override; // 重写图形形状

    double x() const { return m_x; }
    double y() const { return m_y; }
    bool isChecked() const { return checked_; }
    virtual void setChecked(bool checked);
    quint64 index() { return m_index; }
    void setIndex(quint64 index) { m_index = index; }

    // 保存时需要遍历图中所有 item，设置类型以便区分
    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const override { return Type; }

protected:
    QColor color_ = QColor(255, 255, 255, 100); // 结点颜色
    bool checked_ = false; // 是否被选中

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override; // 重写绘制事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // 重写鼠标按下事件

private:
    static constexpr int m_RADIUS = 10; // 图形半径
    QPainterPath m_shape; // 图形形状（圆）
    double m_x; // x 坐标
    double m_y; // y 坐标
    quint64 m_index; // 在图中的索引，用于保存和计算最短路
};

#endif // NODE_H
