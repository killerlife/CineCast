/****************************************************************************
** Meta object code from reading C++ file 'live.h'
**
** Created: Thu Mar 24 14:40:40 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../live.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'live.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Live[] = {

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
      33,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Live[] = {
    "Live\0\0on_radioButton_2_clicked()\0"
    "on_radioButton_clicked()\0"
};

const QMetaObject Live::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Live,
      qt_meta_data_Live, 0 }
};

const QMetaObject *Live::metaObject() const
{
    return &staticMetaObject;
}

void *Live::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Live))
        return static_cast<void*>(const_cast< Live*>(this));
    return QWidget::qt_metacast(_clname);
}

int Live::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_radioButton_2_clicked(); break;
        case 1: on_radioButton_clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
