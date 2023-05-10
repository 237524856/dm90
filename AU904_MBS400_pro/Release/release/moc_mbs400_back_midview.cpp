/****************************************************************************
** Meta object code from reading C++ file 'mbs400_back_midview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MBS_400/mbs400_back_midview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mbs400_back_midview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mbs400_back_midview_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mbs400_back_midview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mbs400_back_midview_t qt_meta_stringdata_mbs400_back_midview = {
    {
QT_MOC_LITERAL(0, 0, 19), // "mbs400_back_midview"
QT_MOC_LITERAL(1, 20, 16), // "LR_chanege_value"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "value"
QT_MOC_LITERAL(4, 44, 10), // "switch_btn"
QT_MOC_LITERAL(5, 55, 17), // "mute_click_status"
QT_MOC_LITERAL(6, 73, 13) // "volume_change"

    },
    "mbs400_back_midview\0LR_chanege_value\0"
    "\0value\0switch_btn\0mute_click_status\0"
    "volume_change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mbs400_back_midview[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       5,    1,   40,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

       0        // eod
};

void mbs400_back_midview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mbs400_back_midview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LR_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->switch_btn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->mute_click_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->volume_change(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mbs400_back_midview::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_mbs400_back_midview.data,
    qt_meta_data_mbs400_back_midview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mbs400_back_midview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mbs400_back_midview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mbs400_back_midview.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int mbs400_back_midview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
