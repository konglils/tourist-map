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
{
    // 开始不可操作，因为没有还没有场景
    setEnabled(false);

    setRenderHint(QPainter::Antialiasing);

    // 关闭默认缩放逻辑，后续自定义缩放
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

void MapView::zoom(bool zoomIn) {
    zoom(zoomIn, viewport()->rect().center());
}

void MapView::open() {
    openFile("/home/cyber/Desktop/ecjtu.map");
}

void MapView::openFile(const QString &filePath) {
    auto map = new TouristMap;
    bool ok = map->openFile(filePath);
    if (ok) {
        showMap(map);
    } else {
        QMessageBox::critical(this, "错误", "地图打开失败");
    }
}

void MapView::save() {
    if (m_map) {
        bool ok = m_map->save();
        if (!ok) {
            QMessageBox::critical(this, "错误", "保存失败");
        }
    }
}

void MapView::createMap(const QString &imageFilePath,
                        const QString &mapTitle,
                        double mapScale) {
    auto map = new TouristMap;
    bool ok = map->setImage(imageFilePath);
    if (ok) {
        map->setTitle(mapTitle);
        map->setScale(mapScale);
        showMap(map);
    } else {
        QMessageBox::critical(this, "错误", "图片读取失败");
    }
}

void MapView::setMode(Mode newMode) {
    if (m_map) {
        switch (m_map->mode()) {
        case SelectMode:
            m_map->clear();
            break;
        case SpotMode:
            m_map->scene()->hideSpotEditor();
            break;
        default:
            break;
        }
        m_map->setMode(newMode);
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
                m_map->scene()->showSpotEditor(mapToScene(event->pos()));
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

void MapView::wheelEvent(QWheelEvent *event) {
    QPoint angle = event->angleDelta();
    if (!angle.isNull()) {
        zoom(angle.y() > 0, event->position());
    }
}

void MapView::showMap(TouristMap *map) {
    delete m_map;
    m_map = map;
    setScene(m_map->scene());
    setTitle(m_map->title());
    setEnabled(true); // 设置 MapView 可操作
}

void MapView::zoom(bool flag, QPointF mousePos) {
    double scal = flag ? zoomFactor : 1 / zoomFactor;

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

void MapView::setTitle(const QString &title) {
    window()->setWindowTitle(QString("%1 - 导游咨询").arg(title));
}
