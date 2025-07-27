#ifndef SPOTEDITOR_H
#define SPOTEDITOR_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>

class SpotEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SpotEditor(QWidget *parent = nullptr);
    QString name() const { return m_name->text(); }
    QString description() const { return m_description->text(); }
    void focusName() { setFocus(); m_name->setFocus(); } // 聚焦在名称输入框

signals:
    void inputEnd(bool focusOut); // 输入结束信号

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLineEdit *m_name; // 名称输入框
    QLineEdit *m_description; // 描述输入框
};

#endif // SPOTEDITOR_H
