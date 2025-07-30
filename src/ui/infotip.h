#ifndef INFOTIP_H
#define INFOTIP_H

#include <QGraphicsTextItem>

#include <QPointF>
#include <QString>

class QGraphicsItem;

/**
 * @brief 信息展示框
 */
class InfoTip : public QGraphicsTextItem
{
public:
    explicit InfoTip(QGraphicsItem *parent = nullptr);

    /// 显示指定文本
    void showText(const QString &text);

    /// 移动显示框到指定位置
    void moveTo(const QPointF &pos);
};

#endif // INFOTIP_H
