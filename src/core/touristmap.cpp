#include "touristmap.h"

#include "global.h"
#include "node.h"
#include "spot.h"
#include "road.h"

#include <queue>

#include <QFile>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QStringDecoder>

TouristMap::TouristMap()
    : m_scene(new MapScene)
{}

bool TouristMap::loadImage() {
    QImage image = QImage::fromData(m_image);
    QPixmap pixmap = QPixmap::fromImage(image);
    if (pixmap.isNull()) {
        return false;
    }
    m_scene->setSceneRect(image.rect());
    auto imageItem = m_scene->addPixmap(pixmap);
    imageItem->setTransformationMode(Qt::SmoothTransformation); // 图片平滑插值
    imageItem->setZValue(-1);

    return true;
}

bool TouristMap::setImage(const QString &imageFileName) {
    QFile file(imageFileName);
    if (file.open(QIODevice::ReadOnly)) {
        m_image = file.readAll();
        return loadImage();
    } else {
        return false;
    }
}

void TouristMap::pressNode(Node *node) {
    switch (m_mode) {
    case SelectMode: {
        node->setChecked(!node->isChecked());
        node->update();

        // 第一次点击节点，会将该节点设为起点，第二次点击另外的节点，会设为终点，然后自动在地图上标注出最短路径
        // 再次点击起点，会重置地图；再次点击终点，会取消选择终点；点击另外的节点，会设置新的终点
        if (node->isChecked()) { // 从未选中到选中
            if (m_source == nullptr) {
                calShortestPath(node);
            } else if (m_destination == nullptr) {
                setPathShow(node, true);
                m_destination = node;
            } else {
                setPathShow(m_destination, false);
                m_destination->setChecked(false);
                m_destination->update();
                setPathShow(node, true);
                m_destination = node;
            }
        } else {
            if (node == m_source) {
                clear();
            } else {
                setPathShow(node, false);
                m_destination = nullptr;
            }
        }
        break;
    }

    case RoadMode: {
        // 路只能以节点开始和结束
        auto buildingRoad = m_scene->buildingRoad();
        if (buildingRoad) {
            buildingRoad->lineTo(node->x(), node->y());
            buildingRoad->render();
            buildingRoad->update();
            buildingRoad->setNode2(node);
            g_map->addRoad(buildingRoad);
            m_scene->setBuildingRoad(nullptr);
        } else {
            buildingRoad = new Road(node->x(), node->y());
            m_scene->setBuildingRoad(buildingRoad);
            buildingRoad->setNode1(node);
            scene()->addItem(buildingRoad);
        }
        break;
    }

    case DelMode:
        delNode(node);
        break;

    default:
        break;
    }
}

void TouristMap::pressRoad(Road *road) {
    switch (m_mode) {
    case SelectMode:
        road->setChecked(!road->isChecked());
        road->update();
        break;

    case DelMode:
        delRoad(road);
        break;

    default:
        break;
    }
}

void TouristMap::clear() {
    if (m_source) {
        m_source->setChecked(false);
        m_source->update();
        if (m_destination) {
            setPathShow(m_destination, false);
            m_destination->setChecked(false);
            m_destination->update();
        }
        m_source = nullptr;
        m_destination = nullptr;
    }
}

void TouristMap::calShortestPath(Node *source) {
    m_shortestDistance.assign(m_graph.size(), std::numeric_limits<double>::max());
    m_shortestPath.assign(m_graph.size(), std::make_pair(nullptr, nullptr));
    m_source = source;
    m_shortestDistance[source->index()] = 0.;

    // Dijkstra
    std::priority_queue<
        std::pair<double, Node *>,
        std::vector<std::pair<double, Node *>>,
        std::greater<>
        > minHeap;
    minHeap.emplace(0., source);

    while (!minHeap.empty()) {
        auto [distance, node] = minHeap.top();
        minHeap.pop();
        if (distance == m_shortestDistance[node->index()]) {
            for (auto [nextNode, road] : m_graph[node->index()]) {
                double newDistance = distance + road->distance();
                if (newDistance < m_shortestDistance[nextNode->index()]) {
                    m_shortestDistance[nextNode->index()] = newDistance;
                    m_shortestPath[nextNode->index()] = { node, road };
                    minHeap.emplace(newDistance, nextNode);
                }
            }
        }
    }
}

void TouristMap::setPathShow(Node *destination, bool show) {
    Node *node = destination;
    while (m_shortestPath[node->index()].first) {
        auto [fromNode, road] = m_shortestPath[node->index()];
        road->setChecked(show);
        road->update();
        node = fromNode;
    }
}

void TouristMap::addNode(Node *node) {
    quint64 index = m_graph.size();
    node->setIndex(index);
    m_graph.emplace_back();
    m_shortestDistance.emplace_back();
    m_shortestPath.emplace_back();
}

void TouristMap::addRoad(Road *road) {
    Node *node1 = road->node1();
    Node *node2 = road->node2();
    m_graph[node1->index()].emplace_back(node2, road);
    m_graph[node2->index()].emplace_back(node1, road);
}

void TouristMap::delNode(Node *node) {
    auto nextSides = m_graph[node->index()]; // delRoad 会修改 m_graph
    for (auto [nextNode, road] : nextSides) {
        delRoad(road);
    }
    m_scene->removeItem(node);
    delete node;
}

void TouristMap::delRoad(Road *road) {
    // 稀疏图，O(1) 时间复杂度
    Node *node1 = road->node1();
    Node *node2 = road->node2();
    if (node1) {
        std::erase(m_graph[node1->index()], std::make_pair(node2, road));
    }
    if (node2) {
        std::erase(m_graph[node2->index()], std::make_pair(node1, road));
    }
    m_scene->removeItem(road);
    m_scene->infoTip()->hide();
    delete road;
}

bool readString(QDataStream &in, QString &str) {
    quint64 length;
    in >> length;
    if (in.status() != QDataStream::Ok) {
        return false;
    }
    QByteArray strData;
    strData.resize(length);
    qint64 bytesRead = in.readRawData(strData.data(), length);
    if (bytesRead != length) {
        return false;
    }
    QStringDecoder decoder(QStringDecoder::Utf8);
    str = decoder(strData);
    if (decoder.hasError()) {
        return false;
    }

    return true;
}

bool readFormatInfo(QDataStream &in) {
    static const char *formatName = "TOURIST MAP";
    for (int i = 0; i < 12; i += 1) {
        char ch;
        in >> ch;
        if (ch != formatName[i]) {
            return false;
        }
    }

    quint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != 0x52391391) {
        return false;
    }

    return true;
}

bool TouristMap::openFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);
    in.setVersion(QDataStream::Qt_6_8);

    if (!readFormatInfo(in)) {
        return false;
    }

    quint64 pictureSize, numNode, numSpot, numRoad;
    double scale;
    in >> pictureSize >> scale >> numNode >> numSpot >> numRoad;
    setScale(scale);
    if (in.status() != QDataStream::Ok) {
        return false;
    }

    m_image.resize(pictureSize);
    quint64 numAllNode = numNode + numSpot;
    m_graph.resize(numAllNode);
    m_shortestDistance.resize(numAllNode);
    m_shortestPath.resize(numAllNode);
    m_nodes.resize(numAllNode);

    if (!readString(in, m_name) ||
        !readNode(in, 0, numNode) ||
        !readSpot(in, numNode, numSpot) ||
        !readRoad(in, numRoad) ||
        !readImage(in))
    {
        return false;
    }

    m_fileName = fileName;
    return true;
}

bool TouristMap::readNode(QDataStream &in, quint64 startIndex, quint64 numNode) {
    for (quint64 i = startIndex; i < startIndex + numNode; i += 1) {
        double x, y;
        in >> x >> y;
        if (in.status() != QDataStream::Ok) {
            return false;
        }
        auto node = new Node(x, y);
        node->setIndex(i);
        m_nodes[i] = node;
        m_scene->addItem(node);
    }

    return true;
}

bool TouristMap::readSpot(QDataStream &in, quint64 startIndex, quint64 numSpot) {
    for (quint64 i = startIndex; i < startIndex + numSpot; i += 1) {
        double x, y;
        QString name, description;
        in >> x >> y;
        if (in.status() != QDataStream::Ok || !readString(in, name) || !readString(in, description)) {
            return false;
        }
        auto spot = new Spot(x, y, name, description);
        spot->setIndex(i);
        m_nodes[i] = spot;
        m_scene->addItem(spot);
    }

    return true;
}

bool TouristMap::readRoad(QDataStream &in, quint64 numRoad) {
    for (quint64 i = 0; i < numRoad; i += 1) {
        quint64 nodeIndex1, nodeIndex2, numPoint;
        double x, y;
        in >> nodeIndex1 >> nodeIndex2 >> numPoint >> x >> y;
        if (in.status() != QDataStream::Ok) {
            return false;
        }

        auto road = new Road(x, y);
        for (quint64 j = 1; j < numPoint; j += 1) {
            in >> x >> y;
            if (in.status() != QDataStream::Ok) {
                delete road;
                return false;
            }
            road->lineTo(x, y);
        }

        Node *node1 = m_nodes[nodeIndex1];
        Node *node2 = m_nodes[nodeIndex2];
        road->setNode1(node1);
        road->setNode2(node2);
        m_graph[nodeIndex1].emplace_back(node2, road);
        m_graph[nodeIndex2].emplace_back(node1, road);

        road->render();
        m_scene->addItem(road);
    }

    return true;
}

bool TouristMap::readImage(QDataStream &in) {
    qint64 bytesRead = in.readRawData(m_image.data(), m_image.size());
    if (bytesRead != m_image.size()) {
        return false;
    }
    return loadImage();
}

bool writeString(QDataStream &out, const QString &str) {
    QByteArray strData = str.toUtf8();
    quint64 length = strData.size();
    out << length;
    qint64 bytesWrite = out.writeRawData(strData.data(), strData.size());
    return bytesWrite == strData.size();
}

bool TouristMap::save() {
    if (m_fileName.isNull()) {
        m_fileName = QFileDialog::getSaveFileName(
            nullptr, "保存", "", "地图 (*.map)");
    }
    if (m_fileName.isNull()) {
        return true;
    } else {
        return saveFile(m_fileName);
    }
}

std::tuple<quint64, quint64, quint64> TouristMap::reCalItems() {
    // 保证保存时 Node 都在 Spot 前面
    quint64 numNode = 0, numSpot = 0, numRoad = 0;
    m_nodes.clear();
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto node = qgraphicsitem_cast<Node *>(item)) {
            numNode += 1;
            node->setIndex(m_nodes.size());
            m_nodes.push_back(node);
        }
    }
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto spot = qgraphicsitem_cast<Spot *>(item)) {
            numSpot += 1;
            spot->setIndex(m_nodes.size());
            m_nodes.push_back(spot);
        } else if (auto road = qgraphicsitem_cast<Road *>(item)) {
            numRoad += 1;
        }
    }
    return { numNode, numSpot, numRoad };
}

void writeFormatInfo(QDataStream &out) {
    static const char *formatName = "TOURIST MAP";
    for (int i = 0; i < 12; i += 1) {
        out << formatName[i];
    }
    quint32 magicNumber = 0x52391391;
    out << magicNumber;
}

bool TouristMap::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream out(&file);
    out.setByteOrder(QDataStream::BigEndian);
    out.setVersion(QDataStream::Qt_6_8);

    quint64 pictureSize = m_image.size();
    auto [numNode, numSpot, numRoad] = reCalItems();
    qDebug() << "Item Count: Node =" << numNode << "|" << "Spot =" << numSpot << "|" << "Road =" << numRoad;

    writeFormatInfo(out);
    out << pictureSize << m_scene->scale() << numNode << numSpot << numRoad;
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    quint64 numAllNode = numNode + numSpot;
    m_shortestDistance.assign(numAllNode, std::numeric_limits<double>::max());
    m_shortestPath.assign(numAllNode, std::make_pair(nullptr, nullptr));
    fill(m_graph.begin(), m_graph.end(), std::vector<std::pair<Node *, Road *>>{});
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto road = qgraphicsitem_cast<Road *>(item)) {
            addRoad(road);
        }
    }
    if (m_source) {
        calShortestPath(m_source);
    }

    if (!writeString(out, m_name) ||
        !writeNode(out, 0, numNode) ||
        !writeSpot(out, numNode, numSpot) ||
        !writeRoad(out) ||
        !writePicture(out))
    {
        return false;
    }

    clear();
    return true;
}

bool TouristMap::writeNode(QDataStream &out, quint64 startIndex, quint64 numNode) {
    for (quint64 i = startIndex; i < startIndex + numNode; i += 1) {
        auto node = static_cast<Node *>(m_nodes[i]);
        out << node->x() << node->y();
    }
    if (out.status() != QDataStream::Ok) {
        return false;
    }

    return true;
}

bool TouristMap::writeSpot(QDataStream &out, quint64 startIndex, quint64 numSpot) {
    for (quint64 i = startIndex; i < startIndex + numSpot; i += 1) {
        auto spot = static_cast<Spot *>(m_nodes[i]);
        out << spot->x() << spot->y();
        writeString(out, spot->name());
        writeString(out, spot->description());
        if (out.status() != QDataStream::Ok) {
            return false;
        }
    }

    return true;
}

bool TouristMap::writeRoad(QDataStream &out) {
    for (QGraphicsItem *item : m_scene->items()) {
        if (auto road = qgraphicsitem_cast<Road *>(item)) {
            out << road->node1()->index();
            out << road->node2()->index();
            auto points = road->points();
            quint64 count = points.size();
            out << count;
            for (auto [x, y] : points) {
                out << x << y;
            }
            if (out.status() != QDataStream::Ok) {
                return false;
            }
        }
    }

    return true;
}

bool TouristMap::writePicture(QDataStream &out) {
    qint64 bytesWrite = out.writeRawData(m_image.data(), m_image.size());
    return bytesWrite == m_image.size();
}
