#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(QObject *parent = nullptr);

    double scale() { return m_scale; }
    void setScale(double scale) { m_scale = scale; }

private:
    double m_scale;
};

#endif // MAPSCENE_H
