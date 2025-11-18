QT += core gui sql
DEFINES += PROJECT_SOURCE_DIR=\\\"$$PWD\\\"


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bookinformationdialog.cpp \
    bookManager.cpp \
    booklistwindow.cpp \
    homewindow.cpp \
    loginwindow.cpp \
    main.cpp \
    rentwindow.cpp \
    returnwindow.cpp \
    signupwindow.cpp

HEADERS += \
    bookinformationdialog.h \
    bookManager.h \
    booklistwindow.h \
    centerhelper.h \
    homewindow.h \
    loginwindow.h \
    rentwindow.h \
    returnwindow.h \
    signupwindow.h

FORMS += \
    bookinformationdialog.ui \
    bookManager.ui \
    booklistwindow.ui \
    homewindow.ui \
    loginwindow.ui \
    rentwindow.ui \
    returnwindow.ui \
    signupwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc