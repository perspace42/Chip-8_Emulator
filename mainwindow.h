#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bindkeys.h"
#include <QMainWindow>
#include "Chip8.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QErrorMessage>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Chip8& emulator, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void keyPressed(Qt::Key key, Chip8& emulatorRef);
    void keyReleased(Qt::Key key, Chip8& emulatorRef);
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

    //Attempt To Run The Next Instruction, If an Exception Results Display The Error Message and Close The CHIP-8 Program
    void emulateCycle() {
        try{
            // Attempt To Execute one Chip-8 instruction per cycle
            emulatorRef.nextInstruction();
            // Update the graphics view
            updateGraphics();
        }
        //If an Exception Results Handle It
        catch(NullOperationException error){
            errorDialog->showMessage(error.what());
            on_actionClose_ROM_triggered();
        }
        catch(UnsupportedLanguageException error){
            errorDialog->showMessage(error.what());
            on_actionClose_ROM_triggered();
        }
        catch(std::out_of_range error){
            errorDialog->showMessage(error.what());
            on_actionClose_ROM_triggered();
        }
    }
    //Update the GraphicsView scene based on the video array in the emulator
    void updateGraphics(){
        ui->graphicsView->scene()->clear();

        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 64; ++x) {
                if (emulatorRef.video[y][x] == 1) {//go through each array member in the video to determine if a pixel should be drawn
                    QGraphicsRectItem* pixel = new QGraphicsRectItem(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);//If a pixel should be drawn, this draws it
                    pixel->setBrush(QBrush(currentColor));//This determines the color the pixels will be drawn based on the variable "currentColor"
                    ui->graphicsView->scene()->addItem(pixel);//Add the pixel to the graphics scene
                }
            }
        }
    }

private:
    Ui::MainWindow *ui;
    BindKeys *bindKeys;
    Chip8& emulatorRef;//Get a refrence to the Chip8 emulator
    bool romLoaded = false;//Bool to determine if a rom has been loaded or not
    bool paused = false;//Bool to determine if the program is paused or not
    QGraphicsScene *scene;//The scene that will be assigned to the graphics view
    QTimer *timer;//A timer for controlling how fast the instructions execute
    QColor currentColor = Qt::white;//A Qcolor to determine the color of the drawn pixels onto the graphics scene
    int cycleSpeed = 0;//An int to determine how may milliseconds have to pass before an instruction can execute
    static constexpr int PIXEL_SIZE = 10;//Enlarges the drawn pixels so they aren't to small on the graphics scene
    QErrorMessage *errorDialog = new QErrorMessage();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
};
#endif // MAINWINDOW_H

