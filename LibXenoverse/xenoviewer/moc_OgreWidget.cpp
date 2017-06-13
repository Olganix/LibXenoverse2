/****************************************************************************
** Meta object code from reading C++ file 'OgreWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "OgreWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OgreWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtOgre__OgreWidget_t {
    QByteArrayData data[11];
    char stringdata[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtOgre__OgreWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtOgre__OgreWidget_t qt_meta_stringdata_QtOgre__OgreWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QtOgre::OgreWidget"
QT_MOC_LITERAL(1, 19, 19), // "swapGeometryVisible"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 19), // "swapSkeletonVisible"
QT_MOC_LITERAL(4, 60, 13), // "playAnimation"
QT_MOC_LITERAL(5, 74, 13), // "stopAnimation"
QT_MOC_LITERAL(6, 88, 14), // "pauseAnimation"
QT_MOC_LITERAL(7, 103, 22), // "previousFrameAnimation"
QT_MOC_LITERAL(8, 126, 18), // "nextFrameAnimation"
QT_MOC_LITERAL(9, 145, 13), // "loopAnimation"
QT_MOC_LITERAL(10, 159, 7) // "checked"

    },
    "QtOgre::OgreWidget\0swapGeometryVisible\0"
    "\0swapSkeletonVisible\0playAnimation\0"
    "stopAnimation\0pauseAnimation\0"
    "previousFrameAnimation\0nextFrameAnimation\0"
    "loopAnimation\0checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtOgre__OgreWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    1,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

void QtOgre::OgreWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OgreWidget *_t = static_cast<OgreWidget *>(_o);
        switch (_id) {
        case 0: _t->swapGeometryVisible(); break;
        case 1: _t->swapSkeletonVisible(); break;
        case 2: _t->playAnimation(); break;
        case 3: _t->stopAnimation(); break;
        case 4: _t->pauseAnimation(); break;
        case 5: _t->previousFrameAnimation(); break;
        case 6: _t->nextFrameAnimation(); break;
        case 7: _t->loopAnimation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject QtOgre::OgreWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QtOgre__OgreWidget.data,
      qt_meta_data_QtOgre__OgreWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QtOgre::OgreWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtOgre::OgreWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtOgre__OgreWidget.stringdata))
        return static_cast<void*>(const_cast< OgreWidget*>(this));
    if (!strcmp(_clname, "Ogre::FrameListener"))
        return static_cast< Ogre::FrameListener*>(const_cast< OgreWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QtOgre::OgreWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
