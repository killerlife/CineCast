/****************************************************************************
** Meta object code from reading C++ file 'content.h'
**
** Created: Thu Mar 24 14:40:41 2016
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../content.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'content.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Content[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      45,    8,    8,    8, 0x08,
      86,    8,    8,    8, 0x08,
     124,    8,    8,    8, 0x08,
     162,    8,    8,    8, 0x08,
     198,    8,    8,    8, 0x08,
     242,    8,    8,    8, 0x08,
     286,    8,    8,    8, 0x08,
     325,  322,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Content[] = {
    "Content\0\0on_pushButton_Refush_Disk_clicked()\0"
    "on_progressBar_CopyDir_valueChanged(int)\0"
    "on_pushButton_DeleteDir_USB_clicked()\0"
    "on_pushButton_DeleteDir_HDD_clicked()\0"
    "on_pushButton_ExporttoUSB_clicked()\0"
    "on_treeWidget_HDD_info_clicked(QModelIndex)\0"
    "on_treeWidget_USB_info_clicked(QModelIndex)\0"
    "on_pushButton_USB_UnMount_clicked()\0"
    "te\0timerEvent(QTimerEvent*)\0"
};

const QMetaObject Content::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Content,
      qt_meta_data_Content, 0 }
};

const QMetaObject *Content::metaObject() const
{
    return &staticMetaObject;
}

void *Content::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Content))
        return static_cast<void*>(const_cast< Content*>(this));
    return QWidget::qt_metacast(_clname);
}

int Content::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_Refush_Disk_clicked(); break;
        case 1: on_progressBar_CopyDir_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_pushButton_DeleteDir_USB_clicked(); break;
        case 3: on_pushButton_DeleteDir_HDD_clicked(); break;
        case 4: on_pushButton_ExporttoUSB_clicked(); break;
        case 5: on_treeWidget_HDD_info_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: on_treeWidget_USB_info_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: on_pushButton_USB_UnMount_clicked(); break;
        case 8: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
