/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created: Sun Jun 5 23:02:08 2016
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
      30,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      47,    6,    6,    6, 0x08,
      74,    6,    6,    6, 0x08,
     106,    6,    6,    6, 0x08,
     136,    6,    6,    6, 0x08,
     166,    6,    6,    6, 0x08,
     202,    6,    6,    6, 0x08,
     238,    6,    6,    6, 0x08,
     278,    6,    6,    6, 0x08,
     318,    6,    6,    6, 0x08,
     353,    6,    6,    6, 0x08,
     389,    6,    6,    6, 0x08,
     423,    6,    6,    6, 0x08,
     461,    6,    6,    6, 0x08,
     499,    6,    6,    6, 0x08,
     532,    6,    6,    6, 0x08,
     567,    6,    6,    6, 0x08,
     604,    6,    6,    6, 0x08,
     639,    6,    6,    6, 0x08,
     672,    6,    6,    6, 0x08,
     699,    6,    6,    6, 0x08,
     726,    6,    6,    6, 0x08,
     753,    6,    6,    6, 0x08,
     779,    6,    6,    6, 0x08,
     805,    6,    6,    6, 0x08,
     831,    6,    6,    6, 0x08,
     857,    6,    6,    6, 0x08,
     883,    6,    6,    6, 0x08,
     909,    6,    6,    6, 0x08,
     936,  933,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Setup[] = {
    "Setup\0\0on_lineEdit_Frequency_editingFinished()\0"
    "on_pushButton_13_clicked()\0"
    "on_pushButtonApply2_2_clicked()\0"
    "on_pushButtonApply2_clicked()\0"
    "on_pushButtonApply1_clicked()\0"
    "on_lineEdit_DNS_4_editingFinished()\0"
    "on_lineEdit_DNS_3_editingFinished()\0"
    "on_lineEdit_GateWay_2_editingFinished()\0"
    "on_lineEdit_Net_AsK_2_editingFinished()\0"
    "on_lineEdit_Ip_2_editingFinished()\0"
    "on_lineEdit_DNS_2_editingFinished()\0"
    "on_lineEdit_DNS_editingFinished()\0"
    "on_lineEdit_GateWay_editingFinished()\0"
    "on_lineEdit_Net_AsK_editingFinished()\0"
    "on_lineEdit_Ip_editingFinished()\0"
    "on_radioButtonDhcp_2_toggled(bool)\0"
    "on_radioButtonStatic_2_toggled(bool)\0"
    "on_radioButtonStatic_toggled(bool)\0"
    "on_radioButtonDhcp_toggled(bool)\0"
    "on_pushButton_12_clicked()\0"
    "on_pushButton_11_clicked()\0"
    "on_pushButton_10_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_5_clicked()\0"
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
        case 0: on_lineEdit_Frequency_editingFinished(); break;
        case 1: on_pushButton_13_clicked(); break;
        case 2: on_pushButtonApply2_2_clicked(); break;
        case 3: on_pushButtonApply2_clicked(); break;
        case 4: on_pushButtonApply1_clicked(); break;
        case 5: on_lineEdit_DNS_4_editingFinished(); break;
        case 6: on_lineEdit_DNS_3_editingFinished(); break;
        case 7: on_lineEdit_GateWay_2_editingFinished(); break;
        case 8: on_lineEdit_Net_AsK_2_editingFinished(); break;
        case 9: on_lineEdit_Ip_2_editingFinished(); break;
        case 10: on_lineEdit_DNS_2_editingFinished(); break;
        case 11: on_lineEdit_DNS_editingFinished(); break;
        case 12: on_lineEdit_GateWay_editingFinished(); break;
        case 13: on_lineEdit_Net_AsK_editingFinished(); break;
        case 14: on_lineEdit_Ip_editingFinished(); break;
        case 15: on_radioButtonDhcp_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: on_radioButtonStatic_2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: on_radioButtonStatic_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: on_radioButtonDhcp_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: on_pushButton_12_clicked(); break;
        case 20: on_pushButton_11_clicked(); break;
        case 21: on_pushButton_10_clicked(); break;
        case 22: on_pushButton_9_clicked(); break;
        case 23: on_pushButton_8_clicked(); break;
        case 24: on_pushButton_7_clicked(); break;
        case 25: on_pushButton_6_clicked(); break;
        case 26: on_pushButton_5_clicked(); break;
        case 27: on_pushButton_2_clicked(); break;
        case 28: on_pushButton_clicked(); break;
        case 29: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 30;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
