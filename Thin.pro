#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T10:01:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thin
TEMPLATE = app
CONFIG += c++11 precompile_header

SOURCES += \
    code-main/main.cpp \
    code-main/mainwindow.cpp \
    code-main/mainwindow_support.cpp \
    code-main/modelviewer.cpp \
    code-main/modelviewer_generator.cpp \
    code-main/mainwindow_support_genrator.cpp \
    code-modeldata/modelmanager.cpp \
    code-modeldata/modelmanager_support.cpp \
    code-modeldata/modelmanager_utility.cpp \
    code-others/geometry.cpp \
    code-others/thinstruct.cpp \
    code-renderer/standarshader.cpp \
    code-renderer/viewmanager.cpp \
    code-tool/cgalholefiller.cpp \
    #code-tool/cgalmachine.cpp \
    code-tool/cgalremesher.cpp \
    code-tool/objLoader.cpp \
    code-others/assemblyinfo.cpp


HEADERS += \
    maintainvar.h \
    mainwindow.h \
    mainwindow_support.h \
    modelviewer.h \
    modelmanager.h \
    looplist.h \
    pch.h \
    pch_cgal.h \
    standarshader.h \
    viewmanager.h \
    #cgalmachine.h \
    cgaltool.h \
    cgalutility.h \
    geometry.h \
    iglmachine.h \
    tiny_obj_loader.h \
    thinstruct.h \
    assemblyinfo.h

PRECOMPILED_HEADER = pch.h pch_cgal.h
QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CXXFLAGS_RELEASE -= -O2

FORMS    += mainwindow.ui

RESOURCES += \
    standarshader.qrc \
    object.qrc

win32 {
    message(win)
    QMAKE_CXXFLAGS += -bigobj

    INCLUDEPATH += $$PWD/externals/include/glew/
    INCLUDEPATH += $$PWD/externals/include/eigen3/
    INCLUDEPATH += $$PWD/externals/include/glm/
    INCLUDEPATH += $$PWD/externals/include/CGAL-4_8_1/
    INCLUDEPATH += $$PWD/externals/include/boost-1_61/
    INCLUDEPATH += $$PWD/externals/include/gmp/
    INCLUDEPATH += $$PWD/externals/include/mpfr/
    LIBS += -lopengl32
    LIBS += $$PWD/externals/lib/x64/glew32s.lib
    #LIBS += $$PWD/externals/lib/x64/glew32.lib
    LIBS += $$PWD/externals/lib/x64/tinyobjloader_140.lib
    #LIBS += $$PWD/externals/lib/x64/tinyobjloader_140_db.lib

    LIBS += $$PWD/externals/lib/x64/libboost_thread-vc140-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/libboost_system-vc140-mt-1_61.lib
    LIBS += $$PWD/externals/lib/x64/CGAL-vc140-mt-4.8.1.lib
    #LIBS += $$PWD/externals/lib/x64/CGAL-vc140-mt-gd-4.8.1.lib

    LIBS += $$PWD/externals/lib/x64/mpir.lib # replace gmp for x64?
    #LIBS += $$PWD/externals/lib/x64/mpir_db.lib
    #LIBS += $$PWD/externals/lib/x64/gmp.lib
    LIBS += $$PWD/externals/lib/x64/mpfr.lib


    QMAKE_LFLAGS_DEBUG = /NODEFAULTLIB:libcmt.lib
    QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:libcmt.lib

}
macx {
    message(macx)

    INCLUDEPATH += /usr/local/include
    INCLUDEPATH += /usr/local/include/eigen3
    LIBS += -L/usr/local/lib -lglew
    LIBS += -lCGAL -lgmp -lmpfr -lboost_thread-mt -lboost_system
    LIBS += $$PWD/externals/lib/mac/libtinyobjloader.a
}
INCLUDEPATH += $$PWD/externals/include/igl

DISTFILES +=
