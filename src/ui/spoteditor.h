#ifndef SPOTEDITOR_H
#define SPOTEDITOR_H

#include <QString>
#include <QWidget>

class QEvent;
class QLineEdit;
class QMouseEvent;

class SpotEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SpotEditor(QWidget *parent = nullptr);
    QString name() const;
    QString description() const;
    void focusName(); // 聚焦在名称输入框
    void clear();

signals:
    // void inputEnd(bool focusOut); // 输入结束信号
    void inputEnd();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLineEdit *m_name; // 名称输入框
    QLineEdit *m_description; // 描述输入框
};

#endif // SPOTEDITOR_H
