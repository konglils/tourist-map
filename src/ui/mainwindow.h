#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MapView;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupActions();

    MapView *m_view;

    QAction *m_currentToggled = nullptr;
};
#endif // MAINWINDOW_H
