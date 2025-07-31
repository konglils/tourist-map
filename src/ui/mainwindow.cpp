#include "mainwindow.h"

#include "mapview.h"
#include "mode.h"

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

    std::vector<std::pair<QAction *, Mode>> actionModes = {
        {nodeAction, NodeMode},
        {spotAction, SpotMode},
        {roadAction, RoadMode},
        {delAction, DelMode},
    };

    for (auto [action, mode] : actionModes) {
        connect(action, &QAction::triggered, [=, this]() {
            if (m_currentToggled) {
                if (m_currentToggled == action) {
                    action->setChecked(false);
                    m_currentToggled = nullptr;
                    m_view->setMode(SelectMode);
                } else {
                    m_currentToggled->setChecked(false);
                    action->setChecked(true);
                    m_currentToggled = action;
                    m_view->setMode(mode);
                }
            } else {
                action->setChecked(true);
                m_currentToggled = action;
                m_view->setMode(mode);
            }
        });
    }

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
