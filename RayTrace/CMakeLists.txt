set(TARGET_NAME raytrace)
file(GLOB_RECURSE RayTrace_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE RayTrace_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui  ${CMAKE_CURRENT_SOURCE_DIR}/res.qrc)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${RayTrace_HEAD} ${RayTrace_SRC})
add_executable(${TARGET_NAME} ${RayTrace_HEAD} ${RayTrace_SRC})


# Link dependencies    
target_link_libraries(${TARGET_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL)



