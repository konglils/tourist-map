/* BUGs
 * 创建 Spot 时，即使点击输入框，鼠标事件还是先会被 MapView 截获，导致重复创建输入框
 * 右键拖拽和滚轮缩放无法同时生效
 * 在较密集的路网周围按住右键拖动转圈，路长信息会闪烁
 * 点击地点，不输入，切换成路口模式再点空白处，输入框不会消失
 */

#include "mainwindow.h"

#include <QApplication>
#include <QImageReader>

int main(int argc, char *argv[])
{
    QImageReader::setAllocationLimit(512);
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
