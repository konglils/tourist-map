#include "mapview.h"

#include "mode.h"
#include "node.h"
#include "spoteditor.h"
#include "touristmap.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWheelEvent>

MapView::MapView(QWidget *parent)
    : QGraphicsView{parent}
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
    if (map()) {
        bool ok = map()->save();
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
    if (map()) {
        switch (map()->mode()) {
        case SelectMode:
            map()->clear();
            break;
        case SpotMode:
            map()->hideSpotEditor();
            break;
        default:
            break;
        }
        map()->setMode(newMode);
    }
}

void MapView::mousePressEvent(QMouseEvent *event) {
    switch (event->button()) {
    case Qt::LeftButton:
        // 事件往下传递时会自动接受，所以先把事件设置为不接受
        // 父类方法返回后检测事件是否被接受，就可以知道事件有没有传给某个元素
        event->setAccepted(false);
        QGraphicsView::mousePressEvent(event);
        // 如果没点击元素
        if (!event->isAccepted()) {
            map()->clickBackground(mapToScene(event->pos()));
        }
        break;

    case Qt::RightButton:
        // 右键按下表示开始拖动地图
        m_dragging = true;
        m_mousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        break;

    default:
        QGraphicsView::mousePressEvent(event);
        break;
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

TouristMap *MapView::map() const {
    return static_cast<TouristMap *>(scene());
}

void MapView::showMap(TouristMap *newMap) {
    delete map();

    setScene(newMap);
    setTitle(newMap->title());

    // 设置 MapView 可操作
    setEnabled(true);
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
