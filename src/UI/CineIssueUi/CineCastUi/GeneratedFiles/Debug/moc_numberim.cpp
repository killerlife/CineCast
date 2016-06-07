/****************************************************************************
** Meta object code from reading C++ file 'numberim.h'
**
** Created: Tue May 31 15:14:05 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../numberim.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'numberim.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NumberIM[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      37,    9,    9,    9, 0x08,
      66,    9,    9,    9, 0x08,
      92,    9,    9,    9, 0x08,
     120,    9,    9,    9, 0x08,
     145,    9,    9,    9, 0x08,
     170,    9,    9,    9, 0x08,
     195,    9,    9,    9, 0x08,
     220,    9,    9,    9, 0x08,
     245,    9,    9,    9, 0x08,
     270,    9,    9,    9, 0x08,
     295,    9,    9,    9, 0x08,
     320,    9,    9,    9, 0x08,
     345,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NumberIM[] = {
    "NumberIM\0\0on_pushButtonDot_clicked()\0"
    "on_pushButtonClose_clicked()\0"
    "on_pushButtonOK_clicked()\0"
    "on_pushButtonBack_clicked()\0"
    "on_pushButton9_clicked()\0"
    "on_pushButton8_clicked()\0"
    "on_pushButton7_clicked()\0"
    "on_pushButton6_clicked()\0"
    "on_pushButton5_clicked()\0"
    "on_pushButton4_clicked()\0"
    "on_pushButton3_clicked()\0"
    "on_pushButton2_clicked()\0"
    "on_pushButton1_clicked()\0"
    "on_pushButton0_clicked()\0"
};

const QMetaObject NumberIM::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NumberIM,
      qt_meta_data_NumberIM, 0 }
};

const QMetaObject *NumberIM::metaObject() const
{
    return &staticMetaObject;
}

void *NumberIM::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NumberIM))
        return static_cast<void*>(const_cast< NumberIM*>(this));
    return QDialog::qt_metacast(_clname);
}

int NumberIM::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButtonDot_clicked(); break;
        case 1: on_pushButtonClose_clicked(); break;
        case 2: on_pushButtonOK_clicked(); break;
        case 3: on_pushButtonBack_clicked(); break;
        case 4: on_pushButton9_clicked(); break;
        case 5: on_pushButton8_clicked(); break;
        case 6: on_pushButton7_clicked(); break;
        case 7: on_pushButton6_clicked(); break;
        case 8: on_pushButton5_clicked(); break;
        case 9: on_pushButton4_clicked(); break;
        case 10: on_pushButton3_clicked(); break;
        case 11: on_pushButton2_clicked(); break;
        case 12: on_pushButton1_clicked(); break;
        case 13: on_pushButton0_clicked(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
