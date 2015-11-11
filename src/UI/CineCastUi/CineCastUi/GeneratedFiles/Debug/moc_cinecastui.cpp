/****************************************************************************
** Meta object code from reading C++ file 'cinecastui.h'
**
** Created: Wed Nov 11 11:11:49 2015
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cinecastui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cinecastui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CineCastUi[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      48,   45,   11,   11, 0x08,
      85,   73,   11,   11, 0x08,
     121,   11,   11,   11, 0x08,
     133,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CineCastUi[] = {
    "CineCastUi\0\0on_tabWidget_currentChanged(int)\0"
    "te\0timerEvent(QTimerEvent*)\0socketError\0"
    "Error(QAbstractSocket::SocketError)\0"
    "Connected()\0UiFilter()\0"
};

const QMetaObject CineCastUi::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CineCastUi,
      qt_meta_data_CineCastUi, 0 }
};

const QMetaObject *CineCastUi::metaObject() const
{
    return &staticMetaObject;
}

void *CineCastUi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CineCastUi))
        return static_cast<void*>(const_cast< CineCastUi*>(this));
    return QWidget::qt_metacast(_clname);
}

int CineCastUi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        case 2: Error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: Connected(); break;
        case 4: UiFilter(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
