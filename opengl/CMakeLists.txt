set(TARGET_NAME opengldemo)
set(opengldemo_HEAD 
    InitOpengl.h 
    asset/all.h 
    asset/asset.h 
    asset/buffer.h 
    asset/fbo.h 
    asset/sampler.h 
    asset/shader.h 
    asset/texture.h 
    asset/vao.h 
    base.h 
    clock.h 
    component/all.h 
    component/animator.h 
    component/camera.h 
    component/component.h 
    component/light.h 
    component/material.h 
    component/mesh.h 
    component/model.h 
    component/transform.h 
    example/scene01.h 
    glwidget.h 
    input.h 
    log.h 
    mainwindow.h 
    pch.h 
    scene/entity.h 
    scene/renderer.h 
    scene/resource.h 
    scene/scene.h 
    stb_image.h 
    syn.h 
    util/ext.h 
    util/image.h 
    util/math.h 
    util/path.h 
    win_dow.h)
set(opengldemo_SOURCE 
    asset/asset.cpp 
    asset/buffer.cpp 
    asset/fbo.cpp 
    asset/sampler.cpp 
    asset/shader.cpp 
    asset/texture.cpp 
    asset/vao.cpp 
    clock.cpp 
    component/animator.cpp 
    component/camera.cpp 
    component/light.cpp 
    component/material.cpp 
    component/mesh.cpp 
    component/model.cpp 
    component/transform.cpp 
    example/scene01.cpp 
    glwidget.cpp 
    input.cpp 
    log.cpp 
    main.cpp 
    mainwindow.cpp 
    scene/renderer.cpp 
    scene/scene.cpp 
    syn.cpp 
    util/image.cpp 
    util/math.cpp 
    util/path.cpp 
    win_dow.cpp
    mainwindow.ui
    res.qrc
    )
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${opengldemo_HEAD} ${opengldemo_SOURCE})
add_executable(${TARGET_NAME} ${opengldemo_HEAD} ${opengldemo_SOURCE})
# Link dependencies   
target_link_directories(${TARGET_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/libs/Debug
)

target_link_libraries(${TARGET_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Opengl32
    assimp-vc143-mtd
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(${TARGET_NAME} PUBLIC
            ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

