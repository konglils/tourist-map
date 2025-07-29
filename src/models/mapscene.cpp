#include "mapscene.h"

#include "infotip.h"
#include "spoteditor.h"
#include "touristmap.h"

#include <QGraphicsProxyWidget>

MapScene::MapScene(TouristMap *map, QObject *parent)
    : QGraphicsScene{parent}
    , m_map{map}
    , m_infoTip{new InfoTip}
    , m_spotEditor{new SpotEditor}
{
    addItem(m_infoTip);

    m_spotEditor->setFixedSize(150, 50);
    m_editorProxy = addWidget(m_spotEditor);
    m_editorProxy->setZValue(10);
    m_editorProxy->setFlag(QGraphicsItem::ItemIgnoresTransformations); // 不随 view 缩放
    m_editorProxy->hide();

    connect(m_spotEditor, &SpotEditor::inputEnd, [this]() {
        QString name = m_spotEditor->name();
        QString description = m_spotEditor->description();

        m_spotEditor->clear();
        m_editorProxy->hide();

        if (!name.isEmpty() || !description.isEmpty()) {
            auto [x, y] = m_editorProxy->pos() - QPoint(15, 15);
            m_map->addSpot(x, y, name, description);
        }
    });
}

void MapScene::showSpotEditor(QPointF pos) {
    m_editorProxy->setPos(pos + QPoint(15, 15));
    m_editorProxy->show();
    m_spotEditor->focusName();
}

void MapScene::hideSpotEditor() {
    m_spotEditor->clear();
    m_editorProxy->hide();
}
