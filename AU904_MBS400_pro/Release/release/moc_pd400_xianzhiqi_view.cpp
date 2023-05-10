/****************************************************************************
** Meta object code from reading C++ file 'pd400_xianzhiqi_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../PD400/pd400_xianzhiqi_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pd400_xianzhiqi_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_pd400_xianzhiqi_view_t {
    QByteArrayData data[13];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pd400_xianzhiqi_view_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pd400_xianzhiqi_view_t qt_meta_stringdata_pd400_xianzhiqi_view = {
    {
QT_MOC_LITERAL(0, 0, 20), // "pd400_xianzhiqi_view"
QT_MOC_LITERAL(1, 21, 19), // "close_xianfuqi_view"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 10), // "hide_click"
QT_MOC_LITERAL(4, 53, 20), // "xianfu_chanege_value"
QT_MOC_LITERAL(5, 74, 12), // "xianfu_value"
QT_MOC_LITERAL(6, 87, 20), // "qidong_chanege_value"
QT_MOC_LITERAL(7, 108, 12), // "qidong_value"
QT_MOC_LITERAL(8, 121, 19), // "huifu_chanege_value"
QT_MOC_LITERAL(9, 141, 11), // "huifu_value"
QT_MOC_LITERAL(10, 153, 11), // "reset_click"
QT_MOC_LITERAL(11, 165, 13), // "confirm_click"
QT_MOC_LITERAL(12, 179, 13) // "handleTimeout"

    },
    "pd400_xianzhiqi_view\0close_xianfuqi_view\0"
    "\0hide_click\0xianfu_chanege_value\0"
    "xianfu_value\0qidong_chanege_value\0"
    "qidong_value\0huifu_chanege_value\0"
    "huifu_value\0reset_click\0confirm_click\0"
    "handleTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pd400_xianzhiqi_view[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x0a /* Public */,
       4,    1,   56,    2, 0x0a /* Public */,
       6,    1,   59,    2, 0x0a /* Public */,
       8,    1,   62,    2, 0x0a /* Public */,
      10,    0,   65,    2, 0x0a /* Public */,
      11,    0,   66,    2, 0x0a /* Public */,
      12,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void pd400_xianzhiqi_view::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<pd400_xianzhiqi_view *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close_xianfuqi_view(); break;
        case 1: _t->hide_click(); break;
        case 2: _t->xianfu_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->qidong_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->huifu_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->reset_click(); break;
        case 6: _t->confirm_click(); break;
        case 7: _t->handleTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (pd400_xianzhiqi_view::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pd400_xianzhiqi_view::close_xianfuqi_view)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject pd400_xianzhiqi_view::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_pd400_xianzhiqi_view.data,
    qt_meta_data_pd400_xianzhiqi_view,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *pd400_xianzhiqi_view::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pd400_xianzhiqi_view::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_pd400_xianzhiqi_view.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int pd400_xianzhiqi_view::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void pd400_xianzhiqi_view::close_xianfuqi_view()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
