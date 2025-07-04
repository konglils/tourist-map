#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mapview.h"

#include <QHBoxLayout>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QAction *m_currentToggled = nullptr;
    MapView *m_view;

    void setupActions();
};
#endif // MAINWINDOW_H
