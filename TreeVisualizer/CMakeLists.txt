cmake_minimum_required(VERSION 3.16)

project(TreeVisualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(TreeVisualizer
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        impl/AVLTree.cpp
        impl/AVLTree.h
        impl/RBTree.cpp
        impl/RBTree.h
        impl/SplayTree.cpp
        impl/SplayTree.h
        impl/BTree.h
        impl/BTree.cpp
        impl/Treap.h
        impl/Treap.cpp
        impl/Visualization.h
        impl/Visualization.cpp
)
target_link_libraries(TreeVisualizer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(TreeVisualizer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TreeVisualizer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
