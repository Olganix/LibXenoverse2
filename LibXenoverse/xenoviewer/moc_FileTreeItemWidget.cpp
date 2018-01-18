/****************************************************************************
** Meta object code from reading C++ file 'FileTreeItemWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FileTreeItemWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileTreeItemWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DoubleSlider_t {
    QByteArrayData data[5];
    char stringdata[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DoubleSlider_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DoubleSlider_t qt_meta_stringdata_DoubleSlider = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DoubleSlider"
QT_MOC_LITERAL(1, 13, 18), // "doubleValueChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "value"
QT_MOC_LITERAL(4, 39, 18) // "notifyValueChanged"

    },
    "DoubleSlider\0doubleValueChanged\0\0value\0"
    "notifyValueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DoubleSlider[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void DoubleSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DoubleSlider *_t = static_cast<DoubleSlider *>(_o);
        switch (_id) {
        case 0: _t->doubleValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->notifyValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DoubleSlider::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DoubleSlider::doubleValueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DoubleSlider::staticMetaObject = {
    { &QSlider::staticMetaObject, qt_meta_stringdata_DoubleSlider.data,
      qt_meta_data_DoubleSlider,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DoubleSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DoubleSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DoubleSlider.stringdata))
        return static_cast<void*>(const_cast< DoubleSlider*>(this));
    return QSlider::qt_metacast(_clname);
}

int DoubleSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DoubleSlider::doubleValueChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MaterialParameterItemWidget_t {
    QByteArrayData data[7];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaterialParameterItemWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaterialParameterItemWidget_t qt_meta_stringdata_MaterialParameterItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 27), // "MaterialParameterItemWidget"
QT_MOC_LITERAL(1, 28, 20), // "checkBoxStateChanged"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 21), // "componentValueChanged"
QT_MOC_LITERAL(4, 72, 6), // "string"
QT_MOC_LITERAL(5, 79, 4), // "name"
QT_MOC_LITERAL(6, 84, 5) // "value"

    },
    "MaterialParameterItemWidget\0"
    "checkBoxStateChanged\0\0componentValueChanged\0"
    "string\0name\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaterialParameterItemWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       3,    2,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Float,    5,    6,

       0        // eod
};

void MaterialParameterItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaterialParameterItemWidget *_t = static_cast<MaterialParameterItemWidget *>(_o);
        switch (_id) {
        case 0: _t->checkBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->componentValueChanged((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject MaterialParameterItemWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaterialParameterItemWidget.data,
      qt_meta_data_MaterialParameterItemWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaterialParameterItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaterialParameterItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaterialParameterItemWidget.stringdata))
        return static_cast<void*>(const_cast< MaterialParameterItemWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaterialParameterItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_MaterialParameterComponentItemWidget_t {
    QByteArrayData data[8];
    char stringdata[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaterialParameterComponentItemWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaterialParameterComponentItemWidget_t qt_meta_stringdata_MaterialParameterComponentItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 36), // "MaterialParameterComponentIte..."
QT_MOC_LITERAL(1, 37, 12), // "valueChanged"
QT_MOC_LITERAL(2, 50, 0), // ""
QT_MOC_LITERAL(3, 51, 6), // "string"
QT_MOC_LITERAL(4, 58, 4), // "name"
QT_MOC_LITERAL(5, 63, 5), // "value"
QT_MOC_LITERAL(6, 69, 16), // "updateLabelValue"
QT_MOC_LITERAL(7, 86, 17) // "updateSliderValue"

    },
    "MaterialParameterComponentItemWidget\0"
    "valueChanged\0\0string\0name\0value\0"
    "updateLabelValue\0updateSliderValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaterialParameterComponentItemWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   34,    2, 0x0a /* Public */,
       7,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void,

       0        // eod
};

void MaterialParameterComponentItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaterialParameterComponentItemWidget *_t = static_cast<MaterialParameterComponentItemWidget *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< string(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->updateLabelValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->updateSliderValue(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MaterialParameterComponentItemWidget::*_t)(string , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MaterialParameterComponentItemWidget::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MaterialParameterComponentItemWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaterialParameterComponentItemWidget.data,
      qt_meta_data_MaterialParameterComponentItemWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaterialParameterComponentItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaterialParameterComponentItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaterialParameterComponentItemWidget.stringdata))
        return static_cast<void*>(const_cast< MaterialParameterComponentItemWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaterialParameterComponentItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MaterialParameterComponentItemWidget::valueChanged(string _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
