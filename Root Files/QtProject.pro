QT += core gui widgets sql printsupport # printsupport para el reporte

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    component.cpp \
    databasemanager.cpp \
    componentdialog.cpp # ¡NUEVO!

HEADERS += \
    mainwindow.h \
    component.h \
    databasemanager.h \
    componentdialog.h # ¡NUEVO!

FORMS += \
    componentdialog.ui \
    mainwindow.ui \
