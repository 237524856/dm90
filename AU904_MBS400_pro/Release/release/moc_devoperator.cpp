/****************************************************************************
** Meta object code from reading C++ file 'devoperator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../devoperator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devoperator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DevOperator_t {
    QByteArrayData data[12];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DevOperator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DevOperator_t qt_meta_stringdata_DevOperator = {
    {
QT_MOC_LITERAL(0, 0, 11), // "DevOperator"
QT_MOC_LITERAL(1, 12, 13), // "connectDevice"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 16), // "disconnectDevice"
QT_MOC_LITERAL(4, 44, 19), // "refresh_main_signal"
QT_MOC_LITERAL(5, 64, 25), // "pd400_refresh_main_signal"
QT_MOC_LITERAL(6, 90, 27), // "pd400_sysreceive_datasignal"
QT_MOC_LITERAL(7, 118, 28), // "pd400_progress_current_value"
QT_MOC_LITERAL(8, 147, 21), // "pd400_break_fail_mess"
QT_MOC_LITERAL(9, 169, 12), // "startSendMsg"
QT_MOC_LITERAL(10, 182, 18), // "pd400_startSendMsg"
QT_MOC_LITERAL(11, 201, 21) // "pd400_startReceiveMsg"

    },
    "DevOperator\0connectDevice\0\0disconnectDevice\0"
    "refresh_main_signal\0pd400_refresh_main_signal\0"
    "pd400_sysreceive_datasignal\0"
    "pd400_progress_current_value\0"
    "pd400_break_fail_mess\0startSendMsg\0"
    "pd400_startSendMsg\0pd400_startReceiveMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DevOperator[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    1,   69,    2, 0x06 /* Public */,
       8,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   75,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x0a /* Public */,
      11,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DevOperator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DevOperator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectDevice(); break;
        case 1: _t->disconnectDevice(); break;
        case 2: _t->refresh_main_signal(); break;
        case 3: _t->pd400_refresh_main_signal(); break;
        case 4: _t->pd400_sysreceive_datasignal(); break;
        case 5: _t->pd400_progress_current_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->pd400_break_fail_mess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->startSendMsg(); break;
        case 8: _t->pd400_startSendMsg(); break;
        case 9: _t->pd400_startReceiveMsg(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DevOperator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::connectDevice)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::disconnectDevice)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::refresh_main_signal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::pd400_refresh_main_signal)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::pd400_sysreceive_datasignal)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::pd400_progress_current_value)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DevOperator::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DevOperator::pd400_break_fail_mess)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DevOperator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DevOperator.data,
    qt_meta_data_DevOperator,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DevOperator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DevOperator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DevOperator.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "CFirmwareUpgradeProtocol"))
        return static_cast< CFirmwareUpgradeProtocol*>(this);
    return QObject::qt_metacast(_clname);
}

int DevOperator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void DevOperator::connectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DevOperator::disconnectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DevOperator::refresh_main_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DevOperator::pd400_refresh_main_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DevOperator::pd400_sysreceive_datasignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DevOperator::pd400_progress_current_value(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DevOperator::pd400_break_fail_mess(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
