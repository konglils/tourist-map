#ifndef SPOT_H
#define SPOT_H

#include "node.h"

#include <QGraphicsItem>
#include <QString>

class QGraphicsSceneHoverEvent;

/**
 * @brief 地点，具有名称和描述
 */
class Spot : public Node
{
public:
    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const override { return Type; }

    Spot(double x, double y,
         const QString &name, const QString &description,
         QGraphicsItem *parent = nullptr);

    /// 重写选中函数，因为地点和路口选中与否的颜色不同
    void setChecked(bool checked) override;

    QString name() { return m_name; }

    QString description() { return m_description; }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    static constexpr QColor unCheckedColor = QColor(255, 215, 0);
    static constexpr QColor checkedColor = QColor(0, 255, 127);

    QString m_name;

    QString m_description;
};

#endif // SPOT_H
