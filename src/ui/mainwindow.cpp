#include "mainwindow.h"

#include "mapview.h"

#include <QTimer>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , m_view{new MapView{this}}
{
    setWindowTitle("导游咨询");

    resize(1200, 750);

    setCentralWidget(m_view);

    setupActions();

    // 窗口初始化完成后调用
    QTimer::singleShot(0, [this](){
        m_view->openDefault();
    });
}

void MainWindow::setupActions() {
    auto openAction = new QAction("打开", this);

    auto saveAction = new QAction("保存", this);

    auto newMapAction = new QAction("新建", this);

    auto zoomInAction = new QAction(this);
    QIcon iconZoomIn(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
    zoomInAction->setIcon(iconZoomIn);

    auto zoomOutAction = new QAction(this);
    QIcon iconZoomOut(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
    zoomOutAction->setIcon(iconZoomOut);

    auto nodeAction = new QAction("路口", this);
    nodeAction->setCheckable(true);

    auto spotAction = new QAction("地点", this);
    spotAction->setCheckable(true);

    auto roadAction = new QAction("道路", this);
    roadAction->setCheckable(true);

    auto delAction = new QAction("删除", this);
    delAction->setCheckable(true);

    connect(openAction, &QAction::triggered, m_view, &MapView::open);

    connect(saveAction, &QAction::triggered, m_view, &MapView::save);

    connect(newMapAction, &QAction::triggered, m_view, &MapView::newMap);

    connect(zoomInAction, &QAction::triggered, m_view, &MapView::zoomIn);

    connect(zoomOutAction, &QAction::triggered, m_view, &MapView::zoomOut);

    // 按照模式枚举的定义顺序，定义模式对应的 Action
    std::vector<QAction *> modeActions = {
        nullptr,
        nodeAction,
        spotAction,
        roadAction,
        delAction,
    };

    for (int i = 0; i < modeActions.size(); i += 1) {
        auto action = modeActions[i];
        if (action) {
            connect(action, &QAction::triggered, [=, this]() {
                m_view->setMode(i);
            });
        }
    }

    auto enableModeActions = [=](bool b) {
        for (auto action : modeActions) {
            if (action) {
                action->setEnabled(b);
            }
        }
    };

    enableModeActions(false);

    connect(m_view, &MapView::modeChanged, [=, this](int mode) {
        if (m_currentToggled) {
            m_currentToggled->setChecked(false);
        }

        if (mode == -1) {
            enableModeActions(false);
        } else {
            if (!modeActions[1]->isChecked()) {
                enableModeActions(true);
            }
            auto action = modeActions[mode];
            if (action) {
                action->setChecked(true);
            }
            m_currentToggled = action;
        }
    });

    auto toolBar = new QToolBar(this);
    toolBar->setMovable(false);
    toolBar->setFloatable(false);
    addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(newMapAction);
    toolBar->addSeparator();
    toolBar->addAction(zoomInAction);
    toolBar->addAction(zoomOutAction);
    toolBar->addSeparator();
    toolBar->addAction(nodeAction);
    toolBar->addAction(spotAction);
    toolBar->addAction(roadAction);
    toolBar->addAction(delAction);
}
