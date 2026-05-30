#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QImage>
#include <QSize>
#include <QDebug>

constexpr int           GRID_SIDE       = 32;
constexpr int           GRID_SCALE      = GRID_SIDE * GRID_SIDE;
constexpr int           FRAMES          = 6572;
constexpr char          FRAMES_PATH[]   = "resources/frames/";
constexpr int           DELAY           = 5; // ms for a timer
constexpr long long     DELAY_ELAPSED   = 33'333'333; // ns for a single frame = 33 ms

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup objs
    timer.setInterval(DELAY);
    // setup drawing
    connect(&timer, &QTimer::timeout, this, [this](){
        if (drawFrame(current_frame))
            current_frame++;
    });
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startAnimation);

    // setup boxes
    ui->gridLayout->setSpacing(0);
    ui->gridLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < GRID_SIDE; ++i) { // row
        for (int j = 0; j < GRID_SIDE; ++j){ // column
            QCheckBox* box = new QCheckBox("");
            box->setFixedWidth(20);
            ui->gridLayout->addWidget(box, i, j);
        }
    }

    // parse frames
    converted_frames.resize(FRAMES);
    for (int i = 1; i <= FRAMES; ++i) {
        QString path = FRAMES_PATH + "output_" % QString::number(i).rightJustified(4, '0') % ".jpg";
        QImage img = QImage(path);
        if (img.isNull())
            qDebug() << "Image " << i << " at " << path << " not found";

        QSize size = QSize(GRID_SIDE, GRID_SIDE);
        img = img.scaled(size);

        converted_frames[i-1].resize(GRID_SCALE);
        for (int j = 0; j < GRID_SCALE; ++j) {
            QRgb rgb = img.pixel({j % GRID_SIDE, j / GRID_SIDE});
            converted_frames[i-1][j] = (qRed(rgb) + qGreen(rgb) + qBlue(rgb)) > 380 ? true : false;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startAnimation()
{
    timer.start();
    elapsed_timer.start();
}

bool MainWindow::drawFrame(int frame_id)
{
    if (frame_id == FRAMES) {
        timer.stop();
        return false;
    }
    if (elapsed_timer.nsecsElapsed() < ((long long) frame_id + 1) * DELAY_ELAPSED)
        return false;
    QVector<bool> frame = converted_frames[frame_id];
    for (int r = 0; r < GRID_SIDE; ++r) {
        for (int k = 0; k < GRID_SIDE; ++k) {
            QCheckBox* box = qobject_cast<QCheckBox*>(ui->gridLayout->itemAtPosition(r, k)->widget());
            auto state = frame[r*GRID_SIDE + k] ? Qt::Checked : Qt::Unchecked;
            if (box->checkState() != state)
                box->setCheckState(state);
        }
    }
    return true;
}
