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
    QAction *m_currentToggled = nullptr;
    MapView *m_view;

    void setupActions();
};
#endif // MAINWINDOW_H
