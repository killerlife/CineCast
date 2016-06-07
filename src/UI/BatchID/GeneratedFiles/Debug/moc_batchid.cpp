/****************************************************************************
** Meta object code from reading C++ file 'batchid.h'
**
** Created: Wed Mar 16 14:54:00 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../batchid.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'batchid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BatchID[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      35,    8,    8,    8, 0x08,
      67,    8,    8,    8, 0x08,
      91,    8,    8,    8, 0x08,
     123,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BatchID[] = {
    "BatchID\0\0on_pushButton_2_clicked()\0"
    "on_pushButtonBrowse_3_clicked()\0"
    "on_pushButton_clicked()\0"
    "on_pushButtonBrowse_2_clicked()\0"
    "on_pushButtonBrowse_clicked()\0"
};

const QMetaObject BatchID::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BatchID,
      qt_meta_data_BatchID, 0 }
};

const QMetaObject *BatchID::metaObject() const
{
    return &staticMetaObject;
}

void *BatchID::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BatchID))
        return static_cast<void*>(const_cast< BatchID*>(this));
    return QDialog::qt_metacast(_clname);
}

int BatchID::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_2_clicked(); break;
        case 1: on_pushButtonBrowse_3_clicked(); break;
        case 2: on_pushButton_clicked(); break;
        case 3: on_pushButtonBrowse_2_clicked(); break;
        case 4: on_pushButtonBrowse_clicked(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
