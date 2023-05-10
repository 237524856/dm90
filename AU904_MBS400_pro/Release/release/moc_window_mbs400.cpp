/****************************************************************************
** Meta object code from reading C++ file 'window_mbs400.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MBS_400/window_mbs400.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'window_mbs400.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_window_mbs400_t {
    QByteArrayData data[9];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_window_mbs400_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_window_mbs400_t qt_meta_stringdata_window_mbs400 = {
    {
QT_MOC_LITERAL(0, 0, 13), // "window_mbs400"
QT_MOC_LITERAL(1, 14, 14), // "sendMsgSignals"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 15), // "OnConnectDevice"
QT_MOC_LITERAL(4, 46, 18), // "OnDisConnectDevice"
QT_MOC_LITERAL(5, 65, 20), // "refresh_user_massage"
QT_MOC_LITERAL(6, 86, 24), // "receive_highswith_status"
QT_MOC_LITERAL(7, 111, 24), // "refresh_user_customaudio"
QT_MOC_LITERAL(8, 136, 15) // "refresh_main_UI"

    },
    "window_mbs400\0sendMsgSignals\0\0"
    "OnConnectDevice\0OnDisConnectDevice\0"
    "refresh_user_massage\0receive_highswith_status\0"
    "refresh_user_customaudio\0refresh_main_UI"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_window_mbs400[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       6,    1,   55,    2, 0x08 /* Private */,
       7,    0,   58,    2, 0x08 /* Private */,
       8,    0,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void window_mbs400::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<window_mbs400 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsgSignals(); break;
        case 1: _t->OnConnectDevice(); break;
        case 2: _t->OnDisConnectDevice(); break;
        case 3: _t->refresh_user_massage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->receive_highswith_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->refresh_user_customaudio(); break;
        case 6: _t->refresh_main_UI(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (window_mbs400::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&window_mbs400::sendMsgSignals)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject window_mbs400::staticMetaObject = { {
    QMetaObject::SuperData::link<basewindow::staticMetaObject>(),
    qt_meta_stringdata_window_mbs400.data,
    qt_meta_data_window_mbs400,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *window_mbs400::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *window_mbs400::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_window_mbs400.stringdata0))
        return static_cast<void*>(this);
    return basewindow::qt_metacast(_clname);
}

int window_mbs400::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = basewindow::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void window_mbs400::sendMsgSignals()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
