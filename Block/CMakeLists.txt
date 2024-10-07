set(Block_SOURCE_FILES
  main.cpp
  block.cpp
  mainwindow.cpp
  mainwindow.ui
)

set(Block_HEADER_FILES
  block.h
  mainwindow.h
)

add_executable(block
  ${Block_SOURCE_FILES}
  ${Block_HEAEDR_FILES}
)



target_link_libraries(block
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
)