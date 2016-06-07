/****************************************************************************
** Meta object code from reading C++ file 'tkr.h'
**
** Created: Mon May 16 11:12:40 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tkr.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tkr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tkr[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      33,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Tkr[] = {
    "Tkr\0\0on_groupBox_2_toggled(bool)\0"
    "on_treeWidget_clicked(QModelIndex)\0"
};

const QMetaObject Tkr::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Tkr,
      qt_meta_data_Tkr, 0 }
};

const QMetaObject *Tkr::metaObject() const
{
    return &staticMetaObject;
}

void *Tkr::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tkr))
        return static_cast<void*>(const_cast< Tkr*>(this));
    return QWidget::qt_metacast(_clname);
}

int Tkr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_groupBox_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: on_treeWidget_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
