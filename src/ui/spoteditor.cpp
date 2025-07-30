#include "spoteditor.h"

#include <QEvent>
#include <QLineEdit>
#include <QMouseEvent>
#include <QObject>
#include <QVBoxLayout>

SpotEditor::SpotEditor(QWidget *parent)
    : QWidget{parent}
    , m_name{new QLineEdit{this}}
    , m_description{new QLineEdit{this}}
{
    m_name->setPlaceholderText("名称");
    m_description->setPlaceholderText("描述");

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_name);
    layout->addWidget(m_description);

    setTabOrder(m_name, m_description);

    // 让整个控件可以接收焦点
    setFocusPolicy(Qt::StrongFocus);

    // 统一管理焦点事件
    installEventFilter(this);
    m_name->installEventFilter(this);
    m_description->installEventFilter(this);

    // 按下回车也结束输入
    connect(m_name, &QLineEdit::returnPressed, [this]() {
        emit inputEnd();
    });
    connect(m_description, &QLineEdit::returnPressed, [this]() {
        emit inputEnd();
    });
}

void SpotEditor::focusName() {
    setFocus();
    m_name->setFocus();
}

void SpotEditor::clear() {
    m_name->clear();
    m_description->clear();
}

QString SpotEditor::name() const {
    return m_name->text();
}

QString SpotEditor::description() const {
    return m_description->text();
}

bool SpotEditor::eventFilter(QObject *watched, QEvent *event) {
    bool inWidget = watched == this || watched == m_name || watched == m_description;
    bool focusOut = event->type() == QEvent::FocusOut;
    bool noFocus = !hasFocus() && !m_name->hasFocus() && !m_description->hasFocus();
    if (inWidget && focusOut && noFocus) {
        emit inputEnd();
    }
    return QWidget::eventFilter(watched, event);
}
