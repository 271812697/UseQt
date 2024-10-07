set(TARGET_NAME DirectX11Init)
file(GLOB DirectX11Init_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB DirectX11Init_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${DirectX11Init_HEAD} ${DirectX11Init_SOURCE})
add_executable(${TARGET_NAME} ${DirectX11Init_HEAD} ${DirectX11Init_SOURCE})
# Link dependencies    
target_link_libraries(${TARGET_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    d3d11
    d3dcompiler
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



