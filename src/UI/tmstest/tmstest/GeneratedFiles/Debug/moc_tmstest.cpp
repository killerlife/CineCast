/****************************************************************************
** Meta object code from reading C++ file 'tmstest.h'
**
** Created: Sat Nov 21 15:40:05 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tmstest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tmstest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_tmstest[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      35,    8,    8,    8, 0x08,
      59,    8,    8,    8, 0x08,
      71,    8,    8,    8, 0x08,
      79,    8,    8,    8, 0x08,
      85,    8,    8,    8, 0x08,
      92,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_tmstest[] = {
    "tmstest\0\0on_pushButton_2_clicked()\0"
    "on_pushButton_clicked()\0connected()\0"
    "login()\0end()\0list()\0ftp()\0"
};

const QMetaObject tmstest::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_tmstest,
      qt_meta_data_tmstest, 0 }
};

const QMetaObject *tmstest::metaObject() const
{
    return &staticMetaObject;
}

void *tmstest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_tmstest))
        return static_cast<void*>(const_cast< tmstest*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int tmstest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_2_clicked(); break;
        case 1: on_pushButton_clicked(); break;
        case 2: connected(); break;
        case 3: login(); break;
        case 4: end(); break;
        case 5: list(); break;
        case 6: ftp(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
