/****************************************************************************
** Meta object code from reading C++ file 'pd400_yasuoqi_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../PD400/pd400_yasuoqi_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pd400_yasuoqi_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_pd400_yasuoqi_view_t {
    QByteArrayData data[8];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pd400_yasuoqi_view_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pd400_yasuoqi_view_t qt_meta_stringdata_pd400_yasuoqi_view = {
    {
QT_MOC_LITERAL(0, 0, 18), // "pd400_yasuoqi_view"
QT_MOC_LITERAL(1, 19, 18), // "close_yasuoqi_view"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 10), // "hide_click"
QT_MOC_LITERAL(4, 50, 20), // "slider_chanege_value"
QT_MOC_LITERAL(5, 71, 5), // "value"
QT_MOC_LITERAL(6, 77, 11), // "reset_click"
QT_MOC_LITERAL(7, 89, 13) // "confirm_click"

    },
    "pd400_yasuoqi_view\0close_yasuoqi_view\0"
    "\0hide_click\0slider_chanege_value\0value\0"
    "reset_click\0confirm_click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pd400_yasuoqi_view[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    1,   41,    2, 0x0a /* Public */,
       6,    0,   44,    2, 0x0a /* Public */,
       7,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void pd400_yasuoqi_view::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<pd400_yasuoqi_view *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close_yasuoqi_view(); break;
        case 1: _t->hide_click(); break;
        case 2: _t->slider_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->reset_click(); break;
        case 4: _t->confirm_click(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (pd400_yasuoqi_view::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pd400_yasuoqi_view::close_yasuoqi_view)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject pd400_yasuoqi_view::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_pd400_yasuoqi_view.data,
    qt_meta_data_pd400_yasuoqi_view,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *pd400_yasuoqi_view::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pd400_yasuoqi_view::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_pd400_yasuoqi_view.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int pd400_yasuoqi_view::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void pd400_yasuoqi_view::close_yasuoqi_view()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
