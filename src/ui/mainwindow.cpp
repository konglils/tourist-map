#include "mainwindow.h"

#include "global.h"
#include "newmapwindow.h"

#include <QFileDialog>
#include <QTimer>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new MapView(this))
{
    setWindowTitle("导游咨询");
    resize(1200, 750);
    setCentralWidget(m_view);

    setupActions();

    QTimer::singleShot(0, this, [this](){ // 窗口初始化完成后调用
        m_view->open();
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

    connect(openAction, &QAction::triggered, [this]() {
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "打开地图",
            "",
            "地图 (*.map)"
            );
        if (!fileName.isNull()) {
            m_view->openFile(fileName);
        }
    });

    connect(saveAction, &QAction::triggered, [this]() {
        m_view->save();
    });

    connect(newMapAction, &QAction::triggered, [this]() {
        NewMapWindow window;
        bool confirm = window.exec();
        if (confirm) {
            m_view->createMap(
                window.getImageFileName(),
                window.getMapName(),
                window.getScale());
        }
    });

    connect(zoomInAction, &QAction::triggered, [this]() {
        m_view->enlarge(true);
    });

    connect(zoomOutAction, &QAction::triggered, [this]() {
        m_view->enlarge(false);
    });

    auto toggle = [this](QAction *action) {
        if (action == m_currentToggled) {
            action->setChecked(false);
            g_mode = SelectMode;
            m_currentToggled = nullptr;
        } else {
            if (m_currentToggled) {
                m_currentToggled->setChecked(false);
            }
            action->setChecked(true);
            m_currentToggled = action;
        }
    };

    std::vector<std::pair<QAction *, Mode>> actionModes = {
        { nodeAction, NodeMode },
        { spotAction, SpotMode },
        { roadAction, RoadMode },
        { delAction, DelMode },
    };

    for (auto [action, mode] : actionModes) {
        connect(action, &QAction::triggered, [=]() {
            if (g_mode == SelectMode && g_map) {
                g_map->clear();
            }
            g_mode = mode;
            toggle(action);
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
