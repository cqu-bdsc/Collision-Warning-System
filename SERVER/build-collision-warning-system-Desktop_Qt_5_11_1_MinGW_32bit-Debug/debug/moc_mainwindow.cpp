/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../collision-warning-system/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[13];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "on_but_start_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 22), // "onUdpStopButtonClicked"
QT_MOC_LITERAL(4, 56, 16), // "onUdpSendMessage"
QT_MOC_LITERAL(5, 73, 14), // "onSendMessageq"
QT_MOC_LITERAL(6, 88, 6), // "result"
QT_MOC_LITERAL(7, 95, 18), // "onUdpAppendMessage"
QT_MOC_LITERAL(8, 114, 4), // "from"
QT_MOC_LITERAL(9, 119, 7), // "message"
QT_MOC_LITERAL(10, 127, 20), // "on_but_getIP_clicked"
QT_MOC_LITERAL(11, 148, 35), // "on_NetInterface_currentIndexC..."
QT_MOC_LITERAL(12, 184, 5) // "index"

    },
    "MainWindow\0on_but_start_clicked\0\0"
    "onUdpStopButtonClicked\0onUdpSendMessage\0"
    "onSendMessageq\0result\0onUdpAppendMessage\0"
    "from\0message\0on_but_getIP_clicked\0"
    "on_NetInterface_currentIndexChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    1,   52,    2, 0x08 /* Private */,
       7,    2,   55,    2, 0x08 /* Private */,
      10,    0,   60,    2, 0x08 /* Private */,
      11,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_but_start_clicked(); break;
        case 1: _t->onUdpStopButtonClicked(); break;
        case 2: _t->onUdpSendMessage(); break;
        case 3: _t->onSendMessageq((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->onUdpAppendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 5: _t->on_but_getIP_clicked(); break;
        case 6: _t->on_NetInterface_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
