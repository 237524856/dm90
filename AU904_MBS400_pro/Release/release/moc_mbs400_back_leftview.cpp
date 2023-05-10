/****************************************************************************
** Meta object code from reading C++ file 'mbs400_back_leftview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MBS_400/mbs400_back_leftview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mbs400_back_leftview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mbs400_back_leftview_t {
    QByteArrayData data[7];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mbs400_back_leftview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mbs400_back_leftview_t qt_meta_stringdata_mbs400_back_leftview = {
    {
QT_MOC_LITERAL(0, 0, 20), // "mbs400_back_leftview"
QT_MOC_LITERAL(1, 21, 18), // "click_userlist_row"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 16), // "perset_btn_click"
QT_MOC_LITERAL(4, 58, 14), // "user_btn_click"
QT_MOC_LITERAL(5, 73, 16), // "preset_btn_click"
QT_MOC_LITERAL(6, 90, 17) // "user_custom_click"

    },
    "mbs400_back_leftview\0click_userlist_row\0"
    "\0perset_btn_click\0user_btn_click\0"
    "preset_btn_click\0user_custom_click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mbs400_back_leftview[] = {

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
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   42,    2, 0x08 /* Private */,
       4,    0,   43,    2, 0x08 /* Private */,
       5,    0,   44,    2, 0x08 /* Private */,
       6,    1,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void mbs400_back_leftview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mbs400_back_leftview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->click_userlist_row((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->perset_btn_click(); break;
        case 2: _t->user_btn_click(); break;
        case 3: _t->preset_btn_click(); break;
        case 4: _t->user_custom_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mbs400_back_leftview::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mbs400_back_leftview::click_userlist_row)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mbs400_back_leftview::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_mbs400_back_leftview.data,
    qt_meta_data_mbs400_back_leftview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mbs400_back_leftview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mbs400_back_leftview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mbs400_back_leftview.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int mbs400_back_leftview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void mbs400_back_leftview::click_userlist_row(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
