/****************************************************************************
** Meta object code from reading C++ file 'copyfilm.h'
**
** Created: Mon May 16 11:12:42 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../copyfilm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'copyfilm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CopyFilm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      19,    9,    9,    9, 0x0a,
      28,    9,    9,    9, 0x08,
      55,   52,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CopyFilm[] = {
    "CopyFilm\0\0reject()\0accept()\0"
    "on_pushButton_clicked()\0te\0"
    "timerEvent(QTimerEvent*)\0"
};

const QMetaObject CopyFilm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CopyFilm,
      qt_meta_data_CopyFilm, 0 }
};

const QMetaObject *CopyFilm::metaObject() const
{
    return &staticMetaObject;
}

void *CopyFilm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CopyFilm))
        return static_cast<void*>(const_cast< CopyFilm*>(this));
    return QDialog::qt_metacast(_clname);
}

int CopyFilm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: reject(); break;
        case 1: accept(); break;
        case 2: on_pushButton_clicked(); break;
        case 3: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
