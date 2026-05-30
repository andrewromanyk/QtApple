#include "mainwindow.h"

#include <QApplication>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("QtApple");
    QIcon icon = QIcon("resources/icon.ico");
    w.setWindowIcon(icon);
    w.setFixedSize(w.size());
    w.statusBar()->setSizeGripEnabled(false);
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    w.show();
    return a.exec();
}
