#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QElapsedTimer>

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
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QVector<bool>> converted_frames;
    QTimer timer = QTimer();
    int current_frame = 0;
    QElapsedTimer elapsed_timer;

public slots:
    void startAnimation();
    bool drawFrame(int frame_id);
};
#endif // MAINWINDOW_H
