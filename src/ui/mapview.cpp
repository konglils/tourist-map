#include "mapview.h"

#include "node.h"
#include "spot.h"
#include "road.h"
#include "global.h"
#include "spoteditor.h"
#include "touristmap.h"

#include <QApplication>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>

MapView::MapView(QWidget *parent)
    : QGraphicsView(parent)
{
    setEnabled(false); // 默认不可操作，因为没有还没有场景
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

void MapView::setTitle(const QString &name) {
    window()->setWindowTitle(QString("%1 - 导游咨询").arg(name));
}

void MapView::open() {
    // TODO 打开默认地图
    openFile("/home/cyber/Desktop/ecjtu.map");
}

void MapView::setNewMap(std::unique_ptr<TouristMap> map) {
    g_map = std::move(map);
    auto oldScene = scene();
    if (oldScene) {
        oldScene->clear();
        oldScene->deleteLater();
    }
    setScene(g_map->scene());
    setTitle(g_map->name());
    g_scale = g_map->scale();
    setEnabled(true); // 设置 MapView 可操作
    resetTransform(); // 重置平移缩放
    g_infoTip = new InfoTip;
    scene()->addItem(g_infoTip);
}

void MapView::openFile(const QString &fileName) {
    auto map = std::make_unique<TouristMap>(this);
    bool success = map->openFile(fileName);
    if (success) {
        setNewMap(std::move(map));
    } else {
        QMessageBox::critical(this, "错误", "地图打开失败");
    }
}

void MapView::save() {
    if (g_map) {
        bool success = g_map->save();
        if (!success) {
            QMessageBox::critical(this, "错误", "保存失败");
        }
    }
}

void MapView::createMap(const QString &imageFileName, const QString &mapName, double mapScale) {
    auto map = std::make_unique<TouristMap>(this);
    bool success = map->setImage(imageFileName);
    if (success) {
        map->setName(mapName);
        map->setScale(mapScale);
        setNewMap(std::move(map));
    } else {
        QMessageBox::critical(this, "错误", "图片读取失败");
    }
}

void MapView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        auto [x, y] = mapToScene(event->pos());
        if (g_mode == NodeMode) {
            auto node = new Node(x, y);
            g_map->addNode(node);
            scene()->addItem(node);
        } else if (g_mode == SpotMode) {
            event->ignore();
            QGraphicsView::mousePressEvent(event);
            if (!event->isAccepted()) {
                inputInfo(mapToScene(event->pos()));
            }
        } else if (g_mode == RoadMode) {
            event->ignore();
            QGraphicsView::mousePressEvent(event); // 事件进入 Node，则 accept；进入 Road，则 ignore
            if (!event->isAccepted() && g_buildingRoad) {
                g_buildingRoad->lineTo(x, y);
                g_buildingRoad->render();
                g_buildingRoad->update();
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

void MapView::inputInfo(QPointF pos) {
    auto editor = new SpotEditor;
    editor->setFixedSize(150, 50);
    auto proxy = scene()->addWidget(editor);
    proxy->setPos(pos + QPoint(15, 15));
    proxy->setZValue(10);
    proxy->setFlag(QGraphicsItem::ItemIgnoresTransformations); // 不随 view 缩放
    editor->focusName();

    connect(editor, &SpotEditor::inputEnd, [=, this](bool focusOut) {
        QString name = editor->name();
        QString description = editor->description();

        scene()->removeItem(proxy); // 会触发 SpotEditor 失焦事件

        if (focusOut) {
            proxy->deleteLater();
            editor->deleteLater();

            if (!name.isEmpty() || !description.isEmpty()) {
                auto spot = new Spot(pos.x(), pos.y(), name, description);
                g_map->addNode(spot);
                scene()->addItem(spot);
            }
        }
    });
}
