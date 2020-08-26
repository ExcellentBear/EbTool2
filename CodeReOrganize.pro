#-------------------------------------------------
#
# Project created by QtCreator 2019-02-22T20:19:32
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeReOrganize
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDE_ROOT_PATH = $$PWD/3rd/include
LIB_ROOT_PATH = $$PWD/3rd/lib
DY_LIB_ROOT_PATH = $$PWD/3rd/dll

INCLUDEPATH += \
        $$INCLUDE_ROOT_PATH/opencv \
        $$INCLUDE_ROOT_PATH/pvt.leptonica \
        $$INCLUDE_ROOT_PATH/tesseract \
        $$INCLUDE_ROOT_PATH/tesseract/src  \
        $$INCLUDE_ROOT_PATH/tesseract/src/api \
        $$INCLUDE_ROOT_PATH/tesseract/src/arch \
        $$INCLUDE_ROOT_PATH/tesseract/src/ccmain \
        $$INCLUDE_ROOT_PATH/tesseract/src/ccstruct \
        $$INCLUDE_ROOT_PATH/tesseract/src/ccutil \
        $$INCLUDE_ROOT_PATH/tesseract/src/classify \
        $$INCLUDE_ROOT_PATH/tesseract/src/cutil \
        $$INCLUDE_ROOT_PATH/tesseract/src/dict \
        $$INCLUDE_ROOT_PATH/tesseract/src/lstm \
        $$INCLUDE_ROOT_PATH/tesseract/src/opencl \
        $$INCLUDE_ROOT_PATH/tesseract/src/textord \
        $$INCLUDE_ROOT_PATH/tesseract/src/training \
        $$INCLUDE_ROOT_PATH/tesseract/src/viewer \
        $$INCLUDE_ROOT_PATH/tesseract/src/vs2010 \
        $$INCLUDE_ROOT_PATH/tesseract/src/wordrec

LIBS += \
    -L$${LIB_ROOT_PATH} \
    -lopencv_world400 \
    -lpvt.cppan.demo.danbloomberg.leptonica-1.76.0 \
    -ltesseract40

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    DrawTools/linehist.cpp \
    DrawTools/linevert.cpp \
    DrawTools/recthollow.cpp \
    CvArg/cvcommon.cpp \
    CvArg/cvapi.cpp \
    CvArg/opencvalgor.cpp \
    DrawTools/mshape.cpp \
    loadconfigure.cpp \
    Widgets/figurewidget.cpp \
    DrawTools/shapefactory.cpp \
    DrawTools/shapedeco.cpp \
    Widgets/ImageDeal/drawwindow.cpp \
    ml/mlapi.cpp \
    ml/examplemodel.cpp \
    ml/directapi.cpp \
    ml/sockapi.cpp \
    ml/apiproxy.cpp \
    events/eventdata.cpp \
    events/ebevent.cpp \
    engines/eventmiddleengine.cpp \
    tesseract_ocr/tessapi.cpp \
    utils/movetext.cpp \
    Widgets/DataEdit/dataeditwindow.cpp

HEADERS += \
        mainwindow.h \
    DrawTools/linehist.h \
    DrawTools/linevert.h \
    DrawTools/recthollow.h \
    CvArg/cvcommon.h \
    CvArg/cvapi.h \
    CvArg/opencvalgor.h \
    utils/mtypes.h \
    DrawTools/mshape.h \
    loadconfigure.h \
    Widgets/figurewidget.h \
    DrawTools/shapefactory.h \
    DrawTools/shapedeco.h \
    Widgets/ImageDeal/drawwindow.h \
    ml/mlapi.h \
    ml/examplemodel.h \
    ml/directapi.h \
    ml/sockapi.h \
    ml/apiproxy.h \
    events/eventdata.h \
    events/ebeventtype.h \
    events/ebevent.h \
    engines/eventmiddleengine.h \
    tesseract_ocr/tessapi.h \
    utils/movetext.h \
    Widgets/DataEdit/dataeditwindow.h

FORMS += \
        mainwindow.ui \
    Widgets/ImageDeal/drawwindow.ui \
    Widgets/DataEdit/dataeditwindow.ui

RESOURCES += \
    respo.qrc

DISTFILES +=
