CONFIG += c++14 qt

QT += core gui widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    paintarea.cpp \
    resizedialog.cpp \
    shapes.cpp \
    commands.cpp \
    shapelistmodel.cpp \
    shapelistview.cpp \
    shapefactory.cpp \
    aboutdialog.cpp

HEADERS += \
    mainwindow.h \
    paintarea.h \
    resizedialog.h \
    enum.h \
    shapes.h \
    commands.h \
    shapelistmodel.h \
    shapelistview.h \
    shapefactory.h \
    aboutdialog.h

DISTFILES +=

RESOURCES += \
    graphics.qrc \
    css.qrc
