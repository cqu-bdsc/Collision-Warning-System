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
    QByteArrayData data[28];
    char stringdata0[381];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "newMessage"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "message"
QT_MOC_LITERAL(4, 31, 10), // "newLogInfo"
QT_MOC_LITERAL(5, 42, 7), // "logInfo"
QT_MOC_LITERAL(6, 50, 20), // "on_but_start_clicked"
QT_MOC_LITERAL(7, 71, 22), // "onUdpStopButtonClicked"
QT_MOC_LITERAL(8, 94, 16), // "onUdpSendMessage"
QT_MOC_LITERAL(9, 111, 14), // "onSendMessageq"
QT_MOC_LITERAL(10, 126, 6), // "result"
QT_MOC_LITERAL(11, 133, 18), // "onUdpAppendMessage"
QT_MOC_LITERAL(12, 152, 4), // "from"
QT_MOC_LITERAL(13, 157, 26), // "on_pushButton_Stop_clicked"
QT_MOC_LITERAL(14, 184, 14), // "addMessageToDB"
QT_MOC_LITERAL(15, 199, 12), // "getTimeStamp"
QT_MOC_LITERAL(16, 212, 35), // "on_NetInterface_currentIndexC..."
QT_MOC_LITERAL(17, 248, 5), // "index"
QT_MOC_LITERAL(18, 254, 10), // "showResult"
QT_MOC_LITERAL(19, 265, 6), // "setRsu"
QT_MOC_LITERAL(20, 272, 9), // "THRESHOLD"
QT_MOC_LITERAL(21, 282, 18), // "DISTANCE_THRESHOLD"
QT_MOC_LITERAL(22, 301, 20), // "setCarOneNowPosition"
QT_MOC_LITERAL(23, 322, 3), // "lon"
QT_MOC_LITERAL(24, 326, 3), // "lat"
QT_MOC_LITERAL(25, 330, 20), // "setCarTwoNowPosition"
QT_MOC_LITERAL(26, 351, 7), // "showLog"
QT_MOC_LITERAL(27, 359, 21) // "on_pushButton_clicked"

    },
    "MainWindow\0newMessage\0\0message\0"
    "newLogInfo\0logInfo\0on_but_start_clicked\0"
    "onUdpStopButtonClicked\0onUdpSendMessage\0"
    "onSendMessageq\0result\0onUdpAppendMessage\0"
    "from\0on_pushButton_Stop_clicked\0"
    "addMessageToDB\0getTimeStamp\0"
    "on_NetInterface_currentIndexChanged\0"
    "index\0showResult\0setRsu\0THRESHOLD\0"
    "DISTANCE_THRESHOLD\0setCarOneNowPosition\0"
    "lon\0lat\0setCarTwoNowPosition\0showLog\0"
    "on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       4,    1,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  105,    2, 0x08 /* Private */,
       7,    0,  106,    2, 0x08 /* Private */,
       8,    0,  107,    2, 0x08 /* Private */,
       9,    1,  108,    2, 0x08 /* Private */,
      11,    2,  111,    2, 0x08 /* Private */,
      13,    0,  116,    2, 0x08 /* Private */,
      14,    1,  117,    2, 0x08 /* Private */,
      15,    0,  120,    2, 0x08 /* Private */,
      16,    1,  121,    2, 0x08 /* Private */,
      18,    1,  124,    2, 0x08 /* Private */,
      19,    2,  127,    2, 0x08 /* Private */,
      22,    2,  132,    2, 0x08 /* Private */,
      25,    2,  137,    2, 0x08 /* Private */,
      26,    1,  142,    2, 0x08 /* Private */,
      27,    0,  145,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,   12,    3,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QJsonObject,    3,
    QMetaType::LongLong,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   20,   21,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   23,   24,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   23,   24,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMessage((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->newLogInfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_but_start_clicked(); break;
        case 3: _t->onUdpStopButtonClicked(); break;
        case 4: _t->onUdpSendMessage(); break;
        case 5: _t->onSendMessageq((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->onUdpAppendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 7: _t->on_pushButton_Stop_clicked(); break;
        case 8: { bool _r = _t->addMessageToDB((*reinterpret_cast< const QJsonObject(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { long long _r = _t->getTimeStamp();
            if (_a[0]) *reinterpret_cast< long long*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->on_NetInterface_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->showResult((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 12: _t->setRsu((*reinterpret_cast< const double(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 13: _t->setCarOneNowPosition((*reinterpret_cast< const double(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 14: _t->setCarTwoNowPosition((*reinterpret_cast< const double(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2]))); break;
        case 15: _t->showLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::newMessage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::newLogInfo)) {
                *result = 1;
                return;
            }
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::newMessage(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::newLogInfo(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
