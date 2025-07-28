#ifndef INFOTIP_H
#define INFOTIP_H

#include <QGraphicsTextItem>

#include <QPointF>
#include <QString>

class QGraphicsItem;

class InfoTip : public QGraphicsTextItem
{
public:
    InfoTip(QGraphicsItem *parent = nullptr);
    void showingText(const QString &text); // 设置显示的文本
    void showAt(const QPointF &pos); // 在指定坐标处显示
};

#endif // INFOTIP_H
