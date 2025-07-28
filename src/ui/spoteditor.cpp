#include "spoteditor.h"

#include <QEvent>
#include <QKeyEvent>

SpotEditor::SpotEditor(QWidget *parent)
    : QWidget{parent}
    , m_name(new QLineEdit(this))
    , m_description(new QLineEdit(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_name->setPlaceholderText("名称");
    m_description->setPlaceholderText("描述");
    layout->addWidget(m_name);
    layout->addWidget(m_description);
    setLayout(layout);
    setTabOrder(m_name, m_description);
    setFocusPolicy(Qt::StrongFocus); // 让整个 widget 可以接收焦点

    this->installEventFilter(this);
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

void SpotEditor::mousePressEvent(QMouseEvent *event) {
    event->accept();
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

void SpotEditor::clear() {
    m_name->clear();
    m_description->clear();
}
