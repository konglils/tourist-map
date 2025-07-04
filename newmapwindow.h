#ifndef NEWMAPWINDOW_H
#define NEWMAPWINDOW_H

#include <QDialog>

class NewMapWindow : public QDialog
{
    Q_OBJECT
public:
    explicit NewMapWindow(QWidget *parent = nullptr);
    QString getImageFileName() { return m_imageFileName; }
    QString getMapName() { return m_mapName; }
    double getScale() { return m_scale; }

private:
    QString m_imageFileName; // 图片文件名
    QString m_mapName; // 地图名称
    double m_scale; // 地图比例尺
};

#endif // NEWMAPWINDOW_H
