cmake_minimum_required(VERSION 3.5)

project(chess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
set(PROJECT_SOURCES
        main.cpp
        logic/game_logic.h logic/game_logic.cpp logic/game_logic.tpp
        logic/piece.h logic/piece.cpp
        logic/cell.h logic/cell.cpp
        logic/point.h logic/point.cpp
        logic/player_color.h logic/player_color.cpp
        windows/game_window.h windows/game_window.cpp
        widgets/board.h widgets/board.cpp
        widgets/cell.h widgets/cell.cpp
        widgets/piece.h widgets/piece.cpp
        widgets/side_panel.h widgets/side_panel.cpp
        widgets/piece_promotion_selector.h widgets/piece_promotion_selector.cpp
        config.h
        ../resources/resource.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(chess
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET chess APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(chess SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(chess
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(chess PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -O0 -force-debug-info")
target_compile_options(chess PRIVATE -Wall -Wextra -pedantic)

set_target_properties(chess PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(chess)
endif()
