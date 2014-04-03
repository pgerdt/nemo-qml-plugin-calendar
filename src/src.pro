TARGET = nemocalendar
PLUGIN_IMPORT_PATH = org/nemomobile/calendar

TEMPLATE = lib
CONFIG += qt plugin hide_symbols

QT += qml
QT -= gui

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
PKGCONFIG += libkcalcoren-qt5 libmkcal-qt5 libical

INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path +=  $$target.path
INSTALLS += qmldir

CONFIG += link_pkgconfig

isEmpty(SRCDIR) SRCDIR = "."

SOURCES += \
    $$SRCDIR/plugin.cpp \
    $$SRCDIR/calendarevent.cpp \
    $$SRCDIR/calendaragendamodel.cpp \
    $$SRCDIR/calendardb.cpp \
    $$SRCDIR/calendareventcache.cpp \
    $$SRCDIR/calendarapi.cpp \
    $$SRCDIR/calendareventquery.cpp \
    $$SRCDIR/calendarnotebookmodel.cpp \

HEADERS += \
    $$SRCDIR/calendarevent.h \
    $$SRCDIR/calendaragendamodel.h \
    $$SRCDIR/calendardb.h \
    $$SRCDIR/calendareventcache.h \
    $$SRCDIR/calendarapi.h \
    $$SRCDIR/calendareventquery.h \
    $$SRCDIR/calendarnotebookmodel.h \

MOC_DIR = $$PWD/.moc
OBJECTS_DIR = $$PWD/.obj
