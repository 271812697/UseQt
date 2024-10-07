set(TARGET_NAME ScreenWidget)
file(GLOB ScreenWidget_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB ScreenWidget_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ScreenWidget_HEAD} ${ScreenWidget_SOURCE})
add_executable(${TARGET_NAME} ${ScreenWidget_HEAD} ${ScreenWidget_SOURCE})
# Link dependencies    
target_link_libraries(${TARGET_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



