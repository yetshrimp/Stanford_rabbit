QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += glad/include/ \
            glfw-3.3/include/ \
            glm-0.9.9.6/

LIBS += -lglfw3 \
     -lGL \
     -lX11 \
     -lpthread \
     -lXrandr \
     -lXi \
     -lXxf86vm \
     -lXinerama \
     -lXxf86vm \
     -lXcursor \
     -lrt \
     -lm

SOURCES += \
        camera.cpp \
        glad.c \
        main.cpp \
        model.cpp \
        ply.cpp \
        shader.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    bunny.ply \
    bunny_iH.ply2 \
    shader.vs \
    shader/lightShader.fs \
    shader/lightShader.vs \
    shader/rabbitShader.fs \
    shader/rabbitShader.vs

HEADERS += \
    callback.h \
    camera.h \
    def.h \
    gl.h \
    model.h \
    ply.h \
    shader.h
