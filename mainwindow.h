#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bindkeys.h"
#include <QMainWindow>
#include "ApplicationLoop.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Chip8& emulator, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionColor_triggered();

    void on_actionSet_Speed_triggered();

    void on_actionLoad_ROM_triggered();

    void on_DecreaseSize_clicked();

    void on_IncreaseSize_clicked();

    void on_actionExit_Program_triggered();

    void on_Pause_toggled(bool arg1);

    void on_actionChange_Keybinds_triggered();

    void on_actionClose_ROM_triggered();

    void emulateCycle() {
        // Execute one Chip-8 instruction per cycle
        emulatorRef.nextInstruction();

        // Update the graphics view
        updateGraphics();
    }

    void updateGraphics(){
        ui->graphicsView->scene()->clear();

        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                if (emulatorRef.video[y][x] == 1) {
                    QGraphicsRectItem* pixel = new QGraphicsRectItem(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
                    pixel->setBrush(QBrush(currentColor));
                    ui->graphicsView->scene()->addItem(pixel);
                }
            }
        }
    }

private:
    Ui::MainWindow *ui;
    BindKeys *bindKeys;
    Chip8& emulatorRef;
    bool romLoaded = false;
    QGraphicsScene *scene;
    QTimer *timer;
    QColor currentColor = Qt::white;
    int cycleSpeed = 2;
    unsigned int video[32][64]{};
    static constexpr int PIXEL_SIZE = 10;
};
#endif // MAINWINDOW_H

