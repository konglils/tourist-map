#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QPointF>
#include <QGraphicsScene>

class InfoTip;
class QGraphicsProxyWidget;
class SpotEditor;
class TouristMap;

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(TouristMap *map, QObject *parent = nullptr);

    TouristMap *map() { return m_map; }

    InfoTip *infoTip() { return m_infoTip; }

    void showSpotEditor(QPointF pos);

    void hideSpotEditor();

private:
    TouristMap *m_map;

    InfoTip *m_infoTip; // 信息展示框

    SpotEditor *m_spotEditor;
    QGraphicsProxyWidget *m_editorProxy;
};

#endif // MAPSCENE_H
