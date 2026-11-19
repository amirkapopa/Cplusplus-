# Qt project configuration file
# Run: qmake LibrarySystem.pro && make

QT += core gui widgets

CONFIG += c++17

TARGET   = LibrarySystem
TEMPLATE = app

# Warn on deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS

# macOS: AGL framework was removed in newer macOS SDKs.
# We remove it from LIBS and suppress the linker warning.
macx {
    LIBS -= -framework AGL
    QMAKE_LFLAGS += -Wl,-w
}
CONFIG += sdk_no_version_check

SOURCES += \
    main.cpp        \
    book.cpp        \
    person.cpp      \
    library.cpp     \
    filehandler.cpp \
    mainwindow.cpp

HEADERS += \
    exceptions.h   \
    book.h         \
    person.h       \
    library.h      \
    filehandler.h  \
    mainwindow.h

# .ui files — qmake runs uic on these to generate ui_mainwindow.h
FORMS += \
    mainwindow.ui
