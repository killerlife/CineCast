/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created: Tue Nov 10 14:09:33 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../setup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Setup[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      53,    6,    6,    6, 0x08,
      79,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Setup[] = {
    "Setup\0\0on_comboBox_ECHs_currentIndexChanged(QString)\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_clicked()\0"
};

const QMetaObject Setup::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Setup,
      qt_meta_data_Setup, 0 }
};

const QMetaObject *Setup::metaObject() const
{
    return &staticMetaObject;
}

void *Setup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Setup))
        return static_cast<void*>(const_cast< Setup*>(this));
    return QWidget::qt_metacast(_clname);
}

int Setup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_comboBox_ECHs_currentIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: on_pushButton_2_clicked(); break;
        case 2: on_pushButton_clicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
