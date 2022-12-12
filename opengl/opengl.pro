QT -= core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    asset/asset.cpp \
    asset/buffer.cpp \
    asset/fbo.cpp \
    asset/sampler.cpp \
    asset/shader.cpp \
    asset/texture.cpp \
    asset/vao.cpp \
    component/animator.cpp \
    component/camera.cpp \
    component/light.cpp \
    component/material.cpp \
    component/mesh.cpp \
    component/model.cpp \
    component/transform.cpp \
    glwidget.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    util/image.cpp \
    util/math.cpp \
    util/path.cpp

HEADERS += \
    InitOpengl.h \
    asset/all.h \
    asset/asset.h \
    asset/buffer.h \
    asset/fbo.h \
    asset/sampler.h \
    asset/shader.h \
    asset/texture.h \
    asset/vao.h \
    base.h \
    component/all.h \
    component/animator.h \
    component/camera.h \
    component/component.h \
    component/light.h \
    component/material.h \
    component/mesh.h \
    component/model.h \
    component/transform.h \
    glwidget.h \
    log.h \
    mainwindow.h \
    pch.h \
    stb_image.h \
    util/ext.h \
    util/image.h \
    util/math.h \
    util/path.h

FORMS += \
    mainwindow.ui

#加上opengl需要的库
win32:LIBS += -lOpengl32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lassimp-vc142-mt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lassimp-vc142-mtd
else:unix: LIBS += -L$$PWD/./ -lassimp-vc142-mt

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
