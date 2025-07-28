#ifndef SPOT_H
#define SPOT_H

#include "node.h"

#include <QGraphicsItem>
#include <QString>

class QGraphicsSceneHoverEvent;

class Spot : public Node
{
public:
    Spot(double x, double y, const QString &name, const QString &description, QGraphicsItem *parent = nullptr);
    void setChecked(bool checked) override; // 重写选中函数，因为 spot 和 node 选中与否的颜色不同
    QString name() { return m_name; }
    QString description() { return m_description; }

    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const override { return Type; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override; // 重写鼠标悬浮进入事件
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override; // 重写鼠标悬浮移动事件
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override; // 重写鼠标悬浮离开事件

private:
    QString m_name; // 名称
    QString m_description; // 描述
};

#endif // SPOT_H
