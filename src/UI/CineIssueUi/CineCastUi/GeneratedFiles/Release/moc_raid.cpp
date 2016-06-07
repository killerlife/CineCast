/****************************************************************************
** Meta object code from reading C++ file 'raid.h'
**
** Created: Thu Mar 24 14:40:40 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../raid.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Raid[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x08,
      32,    5,    5,    5, 0x08,
      58,    5,    5,    5, 0x08,
      84,    5,    5,    5, 0x08,
     113,  110,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Raid[] = {
    "Raid\0\0on_pushButton_3_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_4_clicked()\0te\0"
    "timerEvent(QTimerEvent*)\0"
};

const QMetaObject Raid::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Raid,
      qt_meta_data_Raid, 0 }
};

const QMetaObject *Raid::metaObject() const
{
    return &staticMetaObject;
}

void *Raid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Raid))
        return static_cast<void*>(const_cast< Raid*>(this));
    return QWidget::qt_metacast(_clname);
}

int Raid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_3_clicked(); break;
        case 1: on_pushButton_2_clicked(); break;
        case 2: on_pushButton_5_clicked(); break;
        case 3: on_pushButton_4_clicked(); break;
        case 4: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
