######################################################################
# Automatically generated by qmake (3.1) Thu Dec 28 14:16:38 2017
######################################################################

TEMPLATE = app
TARGET = Project
INCLUDEPATH += .

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


#######################################################################
# QT LIBS
#######################################################################

QT += widgets


#######################################################################
# RESOURCES
#######################################################################

RESOURCES = resources.qrc

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += include/window.h \
    include/textbox.h \
    include/filelist.h \
    include/functions.h \
    include/custombutton.h
SOURCES += src/main.cpp \
    src/window.cpp \
    src/textbox.cpp \
    src/filelist.cpp \
    src/functions.cpp \
    src/custombutton.cpp
