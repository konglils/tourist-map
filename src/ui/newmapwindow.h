#ifndef NEWMAPWINDOW_H
#define NEWMAPWINDOW_H

#include <QDialog>
#include <QString>

class QWidget;

/**
 * @brief 创建新地图的窗口
 */
class NewMapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewMapWindow(QWidget *parent = nullptr);

    QString imageFilePath() { return m_imageFilePath; }

    QString title() { return m_title; }

    double scale() { return m_scale; }

private:
    QString m_imageFilePath;

    QString m_title;

    double m_scale;
};

#endif // NEWMAPWINDOW_H
