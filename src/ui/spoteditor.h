#ifndef SPOTEDITOR_H
#define SPOTEDITOR_H

#include <QString>
#include <QWidget>

class QEvent;
class QLineEdit;
class QMouseEvent;

/**
 * @brief 地点编辑框
 */
class SpotEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SpotEditor(QWidget *parent = nullptr);

    /**
     * @brief 聚焦在名称输入框
     */
    void focusName();

    /**
     * @brief 清除已输入的文字
     */
    void clear();

    QString name() const;

    QString description() const;

signals:
    /**
     * @brief 输入结束的信号
     *
     * 在输入框中按下回车，或焦点离开编辑框都属于输入结束
     */
    void inputEnd();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLineEdit *m_name;

    QLineEdit *m_description;
};

#endif // SPOTEDITOR_H
