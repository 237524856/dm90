/****************************************************************************
** Meta object code from reading C++ file 'pd400_rightview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../PD400/pd400_rightview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pd400_rightview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_pd400_rightview_t {
    QByteArrayData data[9];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pd400_rightview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pd400_rightview_t qt_meta_stringdata_pd400_rightview = {
    {
QT_MOC_LITERAL(0, 0, 15), // "pd400_rightview"
QT_MOC_LITERAL(1, 16, 21), // "show_limitOrcomp_view"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 9), // "change_eq"
QT_MOC_LITERAL(4, 49, 20), // "mic_eq_chanege_value"
QT_MOC_LITERAL(5, 70, 20), // "hunyin_chanege_value"
QT_MOC_LITERAL(6, 91, 16), // "select_btn_click"
QT_MOC_LITERAL(7, 108, 16), // "isopen_btn_click"
QT_MOC_LITERAL(8, 125, 14) // "mute_btn_click"

    },
    "pd400_rightview\0show_limitOrcomp_view\0"
    "\0change_eq\0mic_eq_chanege_value\0"
    "hunyin_chanege_value\0select_btn_click\0"
    "isopen_btn_click\0mute_btn_click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pd400_rightview[] = {

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
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   52,    2, 0x0a /* Public */,
       4,    1,   53,    2, 0x0a /* Public */,
       5,    1,   56,    2, 0x0a /* Public */,
       6,    0,   59,    2, 0x0a /* Public */,
       7,    1,   60,    2, 0x0a /* Public */,
       8,    1,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void pd400_rightview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<pd400_rightview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_limitOrcomp_view((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->change_eq(); break;
        case 2: _t->mic_eq_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->hunyin_chanege_value((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->select_btn_click(); break;
        case 5: _t->isopen_btn_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->mute_btn_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (pd400_rightview::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&pd400_rightview::show_limitOrcomp_view)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject pd400_rightview::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_pd400_rightview.data,
    qt_meta_data_pd400_rightview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *pd400_rightview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pd400_rightview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_pd400_rightview.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int pd400_rightview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void pd400_rightview::show_limitOrcomp_view(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
