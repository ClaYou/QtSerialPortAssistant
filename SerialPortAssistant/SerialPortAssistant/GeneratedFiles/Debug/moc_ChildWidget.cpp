/****************************************************************************
** Meta object code from reading C++ file 'ChildWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ChildWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChildWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChildWidget_t {
    QByteArrayData data[23];
    char stringdata0[339];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChildWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChildWidget_t qt_meta_stringdata_ChildWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ChildWidget"
QT_MOC_LITERAL(1, 12, 14), // "commWayChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 17), // "tcpudpModeChanged"
QT_MOC_LITERAL(5, 52, 14), // "sendBtnClicked"
QT_MOC_LITERAL(6, 67, 18), // "sendHistorySelcted"
QT_MOC_LITERAL(7, 86, 10), // "repeatSend"
QT_MOC_LITERAL(8, 97, 5), // "iSend"
QT_MOC_LITERAL(9, 103, 18), // "readDataFromClient"
QT_MOC_LITERAL(10, 122, 6), // "strMsg"
QT_MOC_LITERAL(11, 129, 20), // "newConnectFromServer"
QT_MOC_LITERAL(12, 150, 7), // "strAddr"
QT_MOC_LITERAL(13, 158, 5), // "iport"
QT_MOC_LITERAL(14, 164, 18), // "readDataFromServer"
QT_MOC_LITERAL(15, 183, 22), // "disconnectedFromServer"
QT_MOC_LITERAL(16, 206, 17), // "clientSocketError"
QT_MOC_LITERAL(17, 224, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(18, 253, 11), // "socketError"
QT_MOC_LITERAL(19, 265, 22), // "readDataFromSerialPort"
QT_MOC_LITERAL(20, 288, 15), // "serialPortError"
QT_MOC_LITERAL(21, 304, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(22, 333, 5) // "error"

    },
    "ChildWidget\0commWayChanged\0\0index\0"
    "tcpudpModeChanged\0sendBtnClicked\0"
    "sendHistorySelcted\0repeatSend\0iSend\0"
    "readDataFromClient\0strMsg\0"
    "newConnectFromServer\0strAddr\0iport\0"
    "readDataFromServer\0disconnectedFromServer\0"
    "clientSocketError\0QAbstractSocket::SocketError\0"
    "socketError\0readDataFromSerialPort\0"
    "serialPortError\0QSerialPort::SerialPortError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChildWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08 /* Private */,
       4,    1,   77,    2, 0x08 /* Private */,
       5,    0,   80,    2, 0x08 /* Private */,
       6,    0,   81,    2, 0x08 /* Private */,
       7,    1,   82,    2, 0x08 /* Private */,
       9,    1,   85,    2, 0x08 /* Private */,
      11,    2,   88,    2, 0x08 /* Private */,
      14,    0,   93,    2, 0x08 /* Private */,
      15,    2,   94,    2, 0x08 /* Private */,
      16,    1,   99,    2, 0x08 /* Private */,
      19,    0,  102,    2, 0x08 /* Private */,
      20,    1,  103,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QByteArray,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   12,   13,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,

       0        // eod
};

void ChildWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChildWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commWayChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->tcpudpModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sendBtnClicked(); break;
        case 3: _t->sendHistorySelcted(); break;
        case 4: _t->repeatSend((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->readDataFromClient((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->newConnectFromServer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 7: _t->readDataFromServer(); break;
        case 8: _t->disconnectedFromServer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->clientSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 10: _t->readDataFromSerialPort(); break;
        case 11: _t->serialPortError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChildWidget::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_ChildWidget.data,
    qt_meta_data_ChildWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChildWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChildWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChildWidget.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ChildWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
