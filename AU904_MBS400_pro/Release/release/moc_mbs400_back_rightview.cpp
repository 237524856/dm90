/****************************************************************************
** Meta object code from reading C++ file 'mbs400_back_rightview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MBS_400/mbs400_back_rightview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mbs400_back_rightview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mbs400_back_rightview_t {
    QByteArrayData data[11];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mbs400_back_rightview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mbs400_back_rightview_t qt_meta_stringdata_mbs400_back_rightview = {
    {
QT_MOC_LITERAL(0, 0, 21), // "mbs400_back_rightview"
QT_MOC_LITERAL(1, 22, 13), // "switch_status"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "isopen"
QT_MOC_LITERAL(4, 44, 12), // "user_message"
QT_MOC_LITERAL(5, 57, 18), // "refresh_save_click"
QT_MOC_LITERAL(6, 76, 23), // "verticalSlider_setValue"
QT_MOC_LITERAL(7, 100, 10), // "switch_btn"
QT_MOC_LITERAL(8, 111, 20), // "limiter_slider_click"
QT_MOC_LITERAL(9, 132, 15), // "high_pass_click"
QT_MOC_LITERAL(10, 148, 14) // "low_pass_click"

    },
    "mbs400_back_rightview\0switch_status\0"
    "\0isopen\0user_message\0refresh_save_click\0"
    "verticalSlider_setValue\0switch_btn\0"
    "limiter_slider_click\0high_pass_click\0"
    "low_pass_click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mbs400_back_rightview[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   58,    2, 0x08 /* Private */,
       6,    1,   59,    2, 0x08 /* Private */,
       7,    1,   62,    2, 0x08 /* Private */,
       8,    1,   65,    2, 0x08 /* Private */,
       9,    1,   68,    2, 0x08 /* Private */,
      10,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void mbs400_back_rightview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mbs400_back_rightview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->switch_status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->user_message(); break;
        case 2: _t->refresh_save_click(); break;
        case 3: _t->verticalSlider_setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->switch_btn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->limiter_slider_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->high_pass_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->low_pass_click((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mbs400_back_rightview::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mbs400_back_rightview::switch_status)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (mbs400_back_rightview::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mbs400_back_rightview::user_message)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mbs400_back_rightview::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_mbs400_back_rightview.data,
    qt_meta_data_mbs400_back_rightview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mbs400_back_rightview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mbs400_back_rightview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mbs400_back_rightview.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int mbs400_back_rightview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void mbs400_back_rightview::switch_status(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mbs400_back_rightview::user_message()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
