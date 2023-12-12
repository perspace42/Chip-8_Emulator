#ifndef BINDKEYS_H
#define BINDKEYS_H

#include <QDialog>
#include <QKeyEvent>



namespace Ui {
class BindKeys;
}

class BindKeys : public QDialog
{
    Q_OBJECT

public:
    explicit BindKeys(QWidget *parent = nullptr);
    ~BindKeys();
public slots:
    void handleKeyPress(Qt::Key key, Chip8& EmulatorRef);
    void handleKeyRelease(Qt::Key key, Chip8& EmulatorRef);

private slots:
    void on_Close_clicked();

    void on_keySequenceEdit_1_editingFinished();

    void on_RestoreDefaults_clicked();

    void on_keySequenceEdit_2_editingFinished();

    void on_keySequenceEdit_3_editingFinished();

    void on_keySequenceEdit_4_editingFinished();

    void on_keySequenceEdit_Q_editingFinished();

    void on_keySequenceEdit_W_editingFinished();

    void on_keySequenceEdit_E_editingFinished();

    void on_keySequenceEdit_R_editingFinished();

    void on_keySequenceEdit_A_editingFinished();

    void on_keySequenceEdit_S_editingFinished();

    void on_keySequenceEdit_D_editingFinished();

    void on_keySequenceEdit_F_editingFinished();

    void on_keySequenceEdit_Z_editingFinished();

    void on_keySequenceEdit_X_editingFinished();

    void on_keySequenceEdit_C_editingFinished();

    void on_keySequenceEdit_V_editingFinished();

private:
    Ui::BindKeys *ui;
    Qt::Key tempKey;

};

#endif // BINDKEYS_H
