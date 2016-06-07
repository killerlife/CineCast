/****************************************************************************
** Meta object code from reading C++ file 'disk.h'
**
** Created: Mon May 30 09:27:13 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../disk.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'disk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Disk[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x08,
      40,   37,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Disk[] = {
    "Disk\0\0on_pushButton_format_clicked()\0"
    "te\0timerEvent(QTimerEvent*)\0"
};

const QMetaObject Disk::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Disk,
      qt_meta_data_Disk, 0 }
};

const QMetaObject *Disk::metaObject() const
{
    return &staticMetaObject;
}

void *Disk::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Disk))
        return static_cast<void*>(const_cast< Disk*>(this));
    return QWidget::qt_metacast(_clname);
}

int Disk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_format_clicked(); break;
        case 1: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
