/****************************************************************************
** Meta object code from reading C++ file 'signalplotwin.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../brainstart2/Brainstart2/signalplotwin.h"
#include <QtGui/qtextcursor.h>
#include <QScreen>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signalplotwin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_SignalPlotWin_t {
    uint offsetsAndSizes[26];
    char stringdata0[14];
    char stringdata1[21];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[22];
    char stringdata5[23];
    char stringdata6[19];
    char stringdata7[16];
    char stringdata8[17];
    char stringdata9[10];
    char stringdata10[12];
    char stringdata11[14];
    char stringdata12[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SignalPlotWin_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_SignalPlotWin_t qt_meta_stringdata_SignalPlotWin = {
    {
        QT_MOC_LITERAL(0, 13),  // "SignalPlotWin"
        QT_MOC_LITERAL(14, 20),  // "onstartButtonclicked"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 21),  // "onrecordButtonclicked"
        QT_MOC_LITERAL(58, 21),  // "onzoomInButtonclicked"
        QT_MOC_LITERAL(80, 22),  // "onzoomOutButtonclicked"
        QT_MOC_LITERAL(103, 18),  // "onshowProcessedRaw"
        QT_MOC_LITERAL(122, 15),  // "onLowCutEntered"
        QT_MOC_LITERAL(138, 16),  // "onHighCutEntered"
        QT_MOC_LITERAL(155, 9),  // "updGraphs"
        QT_MOC_LITERAL(165, 11),  // "resizeEvent"
        QT_MOC_LITERAL(177, 13),  // "QResizeEvent*"
        QT_MOC_LITERAL(191, 5)   // "event"
    },
    "SignalPlotWin",
    "onstartButtonclicked",
    "",
    "onrecordButtonclicked",
    "onzoomInButtonclicked",
    "onzoomOutButtonclicked",
    "onshowProcessedRaw",
    "onLowCutEntered",
    "onHighCutEntered",
    "updGraphs",
    "resizeEvent",
    "QResizeEvent*",
    "event"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_SignalPlotWin[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    0,   71,    2, 0x08,    4 /* Private */,
       6,    0,   72,    2, 0x08,    5 /* Private */,
       7,    0,   73,    2, 0x08,    6 /* Private */,
       8,    0,   74,    2, 0x08,    7 /* Private */,
       9,    0,   75,    2, 0x08,    8 /* Private */,
      10,    1,   76,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject SignalPlotWin::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SignalPlotWin.offsetsAndSizes,
    qt_meta_data_SignalPlotWin,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_SignalPlotWin_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SignalPlotWin, std::true_type>,
        // method 'onstartButtonclicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onrecordButtonclicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onzoomInButtonclicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onzoomOutButtonclicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onshowProcessedRaw'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLowCutEntered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onHighCutEntered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updGraphs'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resizeEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QResizeEvent *, std::false_type>
    >,
    nullptr
} };

void SignalPlotWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SignalPlotWin *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onstartButtonclicked(); break;
        case 1: _t->onrecordButtonclicked(); break;
        case 2: _t->onzoomInButtonclicked(); break;
        case 3: _t->onzoomOutButtonclicked(); break;
        case 4: _t->onshowProcessedRaw(); break;
        case 5: _t->onLowCutEntered(); break;
        case 6: _t->onHighCutEntered(); break;
        case 7: _t->updGraphs(); break;
        case 8: _t->resizeEvent((*reinterpret_cast< std::add_pointer_t<QResizeEvent*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *SignalPlotWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SignalPlotWin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SignalPlotWin.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SignalPlotWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE