#include "mainwindow.h"

#include <QApplication>
#include <QImageReader>

int main(int argc, char *argv[]) {
    QImageReader::setAllocationLimit(512);
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
