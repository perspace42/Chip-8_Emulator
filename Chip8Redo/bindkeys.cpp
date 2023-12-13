#include "bindkeys.h"
#include "ui_bindkeys.h"
#include <QKeySequenceEdit>
#include <QMessageBox>

Qt::Key bindKeys[16];

BindKeys::BindKeys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindKeys)
{
    ui->setupUi(this);

    //These 16 lines ensure only 1 key can be bound to each keySequenceEdit
    ui->keySequenceEdit_1->setMaximumSequenceLength(1);
    ui->keySequenceEdit_2->setMaximumSequenceLength(1);
    ui->keySequenceEdit_3->setMaximumSequenceLength(1);
    ui->keySequenceEdit_4->setMaximumSequenceLength(1);
    ui->keySequenceEdit_Q->setMaximumSequenceLength(1);
    ui->keySequenceEdit_W->setMaximumSequenceLength(1);
    ui->keySequenceEdit_E->setMaximumSequenceLength(1);
    ui->keySequenceEdit_R->setMaximumSequenceLength(1);
    ui->keySequenceEdit_A->setMaximumSequenceLength(1);
    ui->keySequenceEdit_S->setMaximumSequenceLength(1);
    ui->keySequenceEdit_D->setMaximumSequenceLength(1);
    ui->keySequenceEdit_F->setMaximumSequenceLength(1);
    ui->keySequenceEdit_Z->setMaximumSequenceLength(1);
    ui->keySequenceEdit_X->setMaximumSequenceLength(1);
    ui->keySequenceEdit_C->setMaximumSequenceLength(1);
    ui->keySequenceEdit_V->setMaximumSequenceLength(1);

    //This assigns the bindKeys array to certain values at startup
    bindKeys[0] = Qt::Key_1;
    bindKeys[1] = Qt::Key_2;
    bindKeys[2] = Qt::Key_3;
    bindKeys[3] = Qt::Key_4;
    bindKeys[4] = Qt::Key_Q;
    bindKeys[5] = Qt::Key_W;
    bindKeys[6] = Qt::Key_E;
    bindKeys[7] = Qt::Key_R;
    bindKeys[8] = Qt::Key_A;
    bindKeys[9] = Qt::Key_S;
    bindKeys[10] = Qt::Key_D;
    bindKeys[11] = Qt::Key_F;
    bindKeys[12] = Qt::Key_Z;
    bindKeys[13] = Qt::Key_X;
    bindKeys[14] = Qt::Key_C;
    bindKeys[15] = Qt::Key_V;

    //this sets the keySequenceEdits to their default key binds
    ui->keySequenceEdit_1->setKeySequence(bindKeys[0]);
    ui->keySequenceEdit_2->setKeySequence(bindKeys[1]);
    ui->keySequenceEdit_3->setKeySequence(bindKeys[2]);
    ui->keySequenceEdit_4->setKeySequence(bindKeys[3]);
    ui->keySequenceEdit_Q->setKeySequence(bindKeys[4]);
    ui->keySequenceEdit_W->setKeySequence(bindKeys[5]);
    ui->keySequenceEdit_E->setKeySequence(bindKeys[6]);
    ui->keySequenceEdit_R->setKeySequence(bindKeys[7]);
    ui->keySequenceEdit_A->setKeySequence(bindKeys[8]);
    ui->keySequenceEdit_S->setKeySequence(bindKeys[9]);
    ui->keySequenceEdit_D->setKeySequence(bindKeys[10]);
    ui->keySequenceEdit_F->setKeySequence(bindKeys[11]);
    ui->keySequenceEdit_Z->setKeySequence(bindKeys[12]);
    ui->keySequenceEdit_X->setKeySequence(bindKeys[13]);
    ui->keySequenceEdit_C->setKeySequence(bindKeys[14]);
    ui->keySequenceEdit_V->setKeySequence(bindKeys[15]);
}

BindKeys::~BindKeys()
{
    delete ui;
}
//Hides the "Change Key Binds" Dialog window
void BindKeys::on_Close_clicked()
{
    this->hide();
}

//Restores the key binds back to their default
void BindKeys::on_RestoreDefaults_clicked()
{
    bindKeys[0] = Qt::Key_1;
    bindKeys[1] = Qt::Key_2;
    bindKeys[2] = Qt::Key_3;
    bindKeys[3] = Qt::Key_4;
    bindKeys[4] = Qt::Key_Q;
    bindKeys[5] = Qt::Key_W;
    bindKeys[6] = Qt::Key_E;
    bindKeys[7] = Qt::Key_R;
    bindKeys[8] = Qt::Key_A;
    bindKeys[9] = Qt::Key_S;
    bindKeys[10] = Qt::Key_D;
    bindKeys[11] = Qt::Key_F;
    bindKeys[12] = Qt::Key_Z;
    bindKeys[13] = Qt::Key_X;
    bindKeys[14] = Qt::Key_C;
    bindKeys[15] = Qt::Key_V;

    ui->keySequenceEdit_1->setKeySequence(bindKeys[0]);
    ui->keySequenceEdit_2->setKeySequence(bindKeys[1]);
    ui->keySequenceEdit_3->setKeySequence(bindKeys[2]);
    ui->keySequenceEdit_4->setKeySequence(bindKeys[3]);
    ui->keySequenceEdit_Q->setKeySequence(bindKeys[4]);
    ui->keySequenceEdit_W->setKeySequence(bindKeys[5]);
    ui->keySequenceEdit_E->setKeySequence(bindKeys[6]);
    ui->keySequenceEdit_R->setKeySequence(bindKeys[7]);
    ui->keySequenceEdit_A->setKeySequence(bindKeys[8]);
    ui->keySequenceEdit_S->setKeySequence(bindKeys[9]);
    ui->keySequenceEdit_D->setKeySequence(bindKeys[10]);
    ui->keySequenceEdit_F->setKeySequence(bindKeys[11]);
    ui->keySequenceEdit_Z->setKeySequence(bindKeys[12]);
    ui->keySequenceEdit_X->setKeySequence(bindKeys[13]);
    ui->keySequenceEdit_C->setKeySequence(bindKeys[14]);
    ui->keySequenceEdit_V->setKeySequence(bindKeys[15]);
}
//This and the following 15 functions check to make sure the key selected in a keySequenceEdit is not the same as any of the keys in the other keySequenceEdits. If it is, an error message
//will appear and the key will be returned to what it was previously. if no error occurs, the appropriate bindKeys variable will be updated to the selected key.
void BindKeys::on_keySequenceEdit_1_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_1->keySequence() == bindKeys[i] && i != 0) { //Throws an error if the key selected matches any key found in the bindKeys array
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_1->keySequence().toString();//converts keySequence to Qt::Key and assigns it to the appropriate bindKeys variable
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[0] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {//If invalid_argument error occurs, restores the keySequenceEdit to what it was previously and displays an error message
        ui->keySequenceEdit_1->setKeySequence(bindKeys[0]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

void BindKeys::on_keySequenceEdit_2_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_2->keySequence() == bindKeys[i] && i != 1) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_2->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[1] = static_cast<Qt::Key>(mainKey);
    }


    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_2->setKeySequence(bindKeys[1]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_3_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_3->keySequence() == bindKeys[i] && i != 2) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_3->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[2] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_3->setKeySequence(bindKeys[2]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_4_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_4->keySequence() == bindKeys[i] && i != 3) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_4->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[3] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_4->setKeySequence(bindKeys[3]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_Q_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_Q->keySequence() == bindKeys[i] && i != 4) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_Q->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[4] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_Q->setKeySequence(bindKeys[4]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_W_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_W->keySequence() == bindKeys[i] && i != 5) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_W->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[5] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_W->setKeySequence(bindKeys[5]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_E_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_E->keySequence() == bindKeys[i] && i != 6) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_E->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[6] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_E->setKeySequence(bindKeys[6]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_R_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_R->keySequence() == bindKeys[i] && i != 7) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_R->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[7] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_R->setKeySequence(bindKeys[7]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_A_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_A->keySequence() == bindKeys[i] && i != 8) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_A->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[8] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_A->setKeySequence(bindKeys[8]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_S_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_S->keySequence() == bindKeys[i] && i != 9) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_S->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[9] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_S->setKeySequence(bindKeys[9]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_D_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_D->keySequence() == bindKeys[i] && i != 10) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_D->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[10] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_D->setKeySequence(bindKeys[10]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_F_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_F->keySequence() == bindKeys[i] && i != 11) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_F->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[11] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_F->setKeySequence(bindKeys[11]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_Z_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_Z->keySequence() == bindKeys[i] && i != 12) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_Z->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[12] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_Z->setKeySequence(bindKeys[12]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_X_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_X->keySequence() == bindKeys[i] && i != 13) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_X->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[13] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_X->setKeySequence(bindKeys[13]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_C_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_C->keySequence() == bindKeys[i] && i != 14) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_C->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[14] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_C->setKeySequence(bindKeys[14]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void BindKeys::on_keySequenceEdit_V_editingFinished()
{
    try {
        for(int i = 0; i < 16; i++) {
            if (ui->keySequenceEdit_V->keySequence() == bindKeys[i] && i != 15) {
                throw std::invalid_argument("This key is already assigned to another action");
            }
        }
        QString keyString = ui->keySequenceEdit_V->keySequence().toString();
        QKeySequence keySequence = QKeySequence(keyString);
        int mainKey = keySequence[0];
        bindKeys[15] = static_cast<Qt::Key>(mainKey);
    }

    catch (std::invalid_argument& e) {
        ui->keySequenceEdit_V->setKeySequence(bindKeys[15]);
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}
