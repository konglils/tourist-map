#include "mapview.h"

#include "mapscene.h"
#include "mode.h"
#include "node.h"
#include "road.h"
#include "spoteditor.h"
#include "touristmap.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
    , m_spotEditor(new SpotEditor)
{
    setEnabled(false); // 默认不可操作，因为没有还没有场景
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
    setTransformationAnchor(QGraphicsView::NoAnchor);

    m_spotEditor->setFixedSize(150, 50);
}

void MapView::setTitle(const QString &name) {
    window()->setWindowTitle(QString("%1 - 导游咨询").arg(name));
}

void MapView::open() {
    // TODO 打开默认地图
    openFile("/home/cyber/Desktop/ecjtu.map");
}

void MapView::setNewMap(TouristMap *map) {
    delete m_map;
    m_map = map;
    auto oldScene = scene();
    if (oldScene) {
        oldScene->clear();
        oldScene->deleteLater();
    }
    setScene(m_map->scene());
    m_scene = m_map->scene();
    setTitle(m_map->name());
    setEnabled(true); // 设置 MapView 可操作
    resetTransform(); // 重置平移缩放
}

void MapView::openFile(const QString &fileName) {
    auto map = new TouristMap;
    bool success = map->openFile(fileName);
    if (success) {
        setNewMap(map);
    } else {
        QMessageBox::critical(this, "错误", "地图打开失败");
    }
}

void MapView::save() {
    if (m_map) {
        bool success = m_map->save();
        if (!success) {
            QMessageBox::critical(this, "错误", "保存失败");
        }
    }
}

void MapView::createMap(const QString &imageFileName, const QString &mapName, double mapScale) {
    auto map = new TouristMap;
    bool success = map->setImage(imageFileName);
    if (success) {
        map->setName(mapName);
        map->setScale(mapScale);
        setNewMap(map);
    } else {
        QMessageBox::critical(this, "错误", "图片读取失败");
    }
}

void MapView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        auto [x, y] = mapToScene(event->pos());
        if (m_map->mode() == NodeMode) {
            auto node = new Node(x, y);
            m_map->addNode(node);
            scene()->addItem(node);
        } else if (m_map->mode() == SpotMode) {
            event->ignore();
            QGraphicsView::mousePressEvent(event);
            if (!event->isAccepted()) {
                m_scene->showSpotEditor(mapToScene(event->pos()));
            }
        } else if (m_map->mode() == RoadMode) {
            event->ignore();
            QGraphicsView::mousePressEvent(event); // 事件进入 Node，则 accept；进入 Road，则 ignore
            auto buildingRoad = m_map->buildingRoad();
            if (!event->isAccepted() && buildingRoad) {
                buildingRoad->lineTo(x, y);
                buildingRoad->render();
                buildingRoad->update();
            }
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    } else if (event->button() == Qt::RightButton) {
        // 右键拖动
        m_dragging = true;
        m_mousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    } else {
        QGraphicsView::mousePressEvent(event);
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging) {
        auto [dx, dy] = event->pos() - m_mousePos;
        m_mousePos = event->pos();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dx);
        verticalScrollBar()->setValue(verticalScrollBar()->value() - dy);
    } else {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event) {
    if (m_dragging && event->button() == Qt::RightButton) {
        m_dragging = false;
        setCursor(Qt::ArrowCursor);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void MapView::enlarge(bool flag) {
    enlarge(flag, viewport()->rect().center());
}

void MapView::enlarge(bool flag, QPointF mousePos) {
    double scal = flag ? m_SCALING : 1 / m_SCALING;

    auto [x, y] = mapToScene(mousePos.toPoint());
    double mx = x * transform().m11();
    double my = y * transform().m22();
    double nx = mx * scal;
    double ny = my * scal;

    auto hBar = horizontalScrollBar();
    auto vBar = verticalScrollBar();
    hBar->setValue(hBar->value() + nx - mx);
    vBar->setValue(vBar->value() + ny - my);

    scale(scal, scal);

    update();
}

void MapView::wheelEvent(QWheelEvent *event) {
    QPoint angle = event->angleDelta();
    if (!angle.isNull()) {
        enlarge(angle.y() > 0, event->position());
    }
}

void MapView::changeMode(Mode newMode) {
    switch (mode()) {
    case SelectMode:
        if (m_map) {
            m_map->clear();
        }
        break;
    case SpotMode:
        m_scene->hideSpotEditor();
        break;
    default:
        break;
    }
    m_map->setMode(newMode);
}

Mode MapView::mode() {
    return m_map->mode();
}
