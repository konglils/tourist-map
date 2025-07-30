#ifndef NODE_H
#define NODE_H

#include "mapitem.h"

#include <QColor>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QRectF>
#include <QtTypes>

class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

/**
 * @brief 节点类，也代表路口
 */
class Node : public MapItem
{
public:
    /// 保存时需要遍历图中所有 item，设置类型以便区分
    enum { Type = QGraphicsItem::UserType + 1 };
    int type() const override { return Type; }

    /// 提供节点坐标以初始化
    Node(double x, double y, QGraphicsItem *parent = nullptr);

    double x() const { return m_x; }
    double y() const { return m_y; }

    virtual void setChecked(bool checked);
    bool isChecked() const { return m_checked; }

    void setIndex(quint64 index) { m_index = index; }
    quint64 index() { return m_index; }

    QPainterPath shape() const override;

    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    bool m_checked = false;

    QColor m_color = unCheckedColor;

private:
    static constexpr int radius = 10;

    static constexpr QColor unCheckedColor = QColor(255, 255, 255, 100);
    static constexpr QColor checkedColor = QColor(0, 0, 0, 200);

    QPainterPath m_shape;

    double m_x;
    double m_y;

    quint64 m_index; ///< 在图中的索引，用于计算最短路和保存成文件
};

#endif // NODE_H
