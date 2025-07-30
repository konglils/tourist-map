#ifndef NEWMAPWINDOW_H
#define NEWMAPWINDOW_H

#include <QDialog>
#include <QString>

class QWidget;

class NewMapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewMapWindow(QWidget *parent = nullptr);

    QString imageFilePath() { return m_imageFilePath; }

    QString mapTitle() { return m_mapTitle; }

    double scale() { return m_scale; }

private:
    QString m_imageFilePath;

    QString m_mapTitle;

    double m_scale;
};

#endif // NEWMAPWINDOW_H
