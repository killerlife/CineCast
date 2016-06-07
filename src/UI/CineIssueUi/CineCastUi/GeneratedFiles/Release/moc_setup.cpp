/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created: Mon May 16 11:07:21 2016
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
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      34,    6,    6,    6, 0x08,
      61,    6,    6,    6, 0x08,
      88,    6,    6,    6, 0x08,
     114,    6,    6,    6, 0x08,
     140,    6,    6,    6, 0x08,
     166,    6,    6,    6, 0x08,
     192,    6,    6,    6, 0x08,
     218,    6,    6,    6, 0x08,
     264,    6,    6,    6, 0x08,
     290,    6,    6,    6, 0x08,
     317,  314,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Setup[] = {
    "Setup\0\0on_pushButton_12_clicked()\0"
    "on_pushButton_11_clicked()\0"
    "on_pushButton_10_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_comboBox_ECHs_currentIndexChanged(QString)\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_clicked()\0te\0"
    "timerEvent(QTimerEvent*)\0"
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
        case 0: on_pushButton_12_clicked(); break;
        case 1: on_pushButton_11_clicked(); break;
        case 2: on_pushButton_10_clicked(); break;
        case 3: on_pushButton_9_clicked(); break;
        case 4: on_pushButton_8_clicked(); break;
        case 5: on_pushButton_7_clicked(); break;
        case 6: on_pushButton_6_clicked(); break;
        case 7: on_pushButton_5_clicked(); break;
        case 8: on_comboBox_ECHs_currentIndexChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: on_pushButton_2_clicked(); break;
        case 10: on_pushButton_clicked(); break;
        case 11: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
