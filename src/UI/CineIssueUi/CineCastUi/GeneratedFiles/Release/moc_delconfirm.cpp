/****************************************************************************
** Meta object code from reading C++ file 'delconfirm.h'
**
** Created: Thu Mar 24 14:40:41 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../delconfirm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'delconfirm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DelConfirm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      42,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DelConfirm[] = {
    "DelConfirm\0\0on_pushButtonDelete_clicked()\0"
    "on_pushButtonCancel_clicked()\0"
};

const QMetaObject DelConfirm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DelConfirm,
      qt_meta_data_DelConfirm, 0 }
};

const QMetaObject *DelConfirm::metaObject() const
{
    return &staticMetaObject;
}

void *DelConfirm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DelConfirm))
        return static_cast<void*>(const_cast< DelConfirm*>(this));
    return QDialog::qt_metacast(_clname);
}

int DelConfirm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButtonDelete_clicked(); break;
        case 1: on_pushButtonCancel_clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
