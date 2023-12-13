/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:/Users/field/Desktop/Desktop Documents/College (Ivy Tech)/A1 Current Classes/System Software Analysis & Projects/Final Project Chip8 Emulator/Chip-8_Emulator/Chip8Redo/mainwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "keyPressed",
    "",
    "Qt::Key",
    "key",
    "Chip8&",
    "emulatorRef",
    "keyReleased",
    "on_actionColor_triggered",
    "on_actionSet_Speed_triggered",
    "on_actionLoad_ROM_triggered",
    "on_DecreaseSize_clicked",
    "on_IncreaseSize_clicked",
    "on_actionExit_Program_triggered",
    "on_Pause_toggled",
    "arg1",
    "on_actionChange_Keybinds_triggered",
    "on_actionClose_ROM_triggered",
    "emulateCycle",
    "updateGraphics"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[40];
    char stringdata0[11];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[4];
    char stringdata5[7];
    char stringdata6[12];
    char stringdata7[12];
    char stringdata8[25];
    char stringdata9[29];
    char stringdata10[28];
    char stringdata11[24];
    char stringdata12[24];
    char stringdata13[32];
    char stringdata14[17];
    char stringdata15[5];
    char stringdata16[35];
    char stringdata17[29];
    char stringdata18[13];
    char stringdata19[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 10),  // "keyPressed"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 7),  // "Qt::Key"
        QT_MOC_LITERAL(31, 3),  // "key"
        QT_MOC_LITERAL(35, 6),  // "Chip8&"
        QT_MOC_LITERAL(42, 11),  // "emulatorRef"
        QT_MOC_LITERAL(54, 11),  // "keyReleased"
        QT_MOC_LITERAL(66, 24),  // "on_actionColor_triggered"
        QT_MOC_LITERAL(91, 28),  // "on_actionSet_Speed_triggered"
        QT_MOC_LITERAL(120, 27),  // "on_actionLoad_ROM_triggered"
        QT_MOC_LITERAL(148, 23),  // "on_DecreaseSize_clicked"
        QT_MOC_LITERAL(172, 23),  // "on_IncreaseSize_clicked"
        QT_MOC_LITERAL(196, 31),  // "on_actionExit_Program_triggered"
        QT_MOC_LITERAL(228, 16),  // "on_Pause_toggled"
        QT_MOC_LITERAL(245, 4),  // "arg1"
        QT_MOC_LITERAL(250, 34),  // "on_actionChange_Keybinds_trig..."
        QT_MOC_LITERAL(285, 28),  // "on_actionClose_ROM_triggered"
        QT_MOC_LITERAL(314, 12),  // "emulateCycle"
        QT_MOC_LITERAL(327, 14)   // "updateGraphics"
    },
    "MainWindow",
    "keyPressed",
    "",
    "Qt::Key",
    "key",
    "Chip8&",
    "emulatorRef",
    "keyReleased",
    "on_actionColor_triggered",
    "on_actionSet_Speed_triggered",
    "on_actionLoad_ROM_triggered",
    "on_DecreaseSize_clicked",
    "on_IncreaseSize_clicked",
    "on_actionExit_Program_triggered",
    "on_Pause_toggled",
    "arg1",
    "on_actionChange_Keybinds_triggered",
    "on_actionClose_ROM_triggered",
    "emulateCycle",
    "updateGraphics"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   92,    2, 0x06,    1 /* Public */,
       7,    2,   97,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,  102,    2, 0x08,    7 /* Private */,
       9,    0,  103,    2, 0x08,    8 /* Private */,
      10,    0,  104,    2, 0x08,    9 /* Private */,
      11,    0,  105,    2, 0x08,   10 /* Private */,
      12,    0,  106,    2, 0x08,   11 /* Private */,
      13,    0,  107,    2, 0x08,   12 /* Private */,
      14,    1,  108,    2, 0x08,   13 /* Private */,
      16,    0,  111,    2, 0x08,   15 /* Private */,
      17,    0,  112,    2, 0x08,   16 /* Private */,
      18,    0,  113,    2, 0x08,   17 /* Private */,
      19,    0,  114,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'keyPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::Key, std::false_type>,
        QtPrivate::TypeAndForceComplete<Chip8 &, std::false_type>,
        // method 'keyReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::Key, std::false_type>,
        QtPrivate::TypeAndForceComplete<Chip8 &, std::false_type>,
        // method 'on_actionColor_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionSet_Speed_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionLoad_ROM_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_DecreaseSize_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_IncreaseSize_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionExit_Program_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_Pause_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_actionChange_Keybinds_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_actionClose_ROM_triggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'emulateCycle'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateGraphics'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->keyPressed((*reinterpret_cast< std::add_pointer_t<Qt::Key>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Chip8&>>(_a[2]))); break;
        case 1: _t->keyReleased((*reinterpret_cast< std::add_pointer_t<Qt::Key>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Chip8&>>(_a[2]))); break;
        case 2: _t->on_actionColor_triggered(); break;
        case 3: _t->on_actionSet_Speed_triggered(); break;
        case 4: _t->on_actionLoad_ROM_triggered(); break;
        case 5: _t->on_DecreaseSize_clicked(); break;
        case 6: _t->on_IncreaseSize_clicked(); break;
        case 7: _t->on_actionExit_Program_triggered(); break;
        case 8: _t->on_Pause_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->on_actionChange_Keybinds_triggered(); break;
        case 10: _t->on_actionClose_ROM_triggered(); break;
        case 11: _t->emulateCycle(); break;
        case 12: _t->updateGraphics(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(Qt::Key , Chip8 & );
            if (_t _q_method = &MainWindow::keyPressed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(Qt::Key , Chip8 & );
            if (_t _q_method = &MainWindow::keyReleased; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::keyPressed(Qt::Key _t1, Chip8 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::keyReleased(Qt::Key _t1, Chip8 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
