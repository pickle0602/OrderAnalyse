cmake_minimum_required(VERSION 3.16)

project(OrderAnalyse-GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES OrderAnalyse-GUI_zh_CN.ts)

set(PROJECT_SOURCES_GUI
        src/main_gui.cpp
        src/main_widget.cpp
        include/gui/main_widget.h
        src/main_widget.ui
        src/chart_widget.cpp
        src/chart_widget.ui
        include/gui/chart_widget.h
        src/matching_counter_widget.cpp
        src/filter_widget.cpp
        include/gui/matching_counter_widget.h
        include/gui/filter_widget.h
        src/matching_counter_widget.ui
        src/filter_widget.ui
        ${TS_FILES}
)
set(PROJECT_SOURCES_CLI
        src/main_cli.cpp
        ${TS_FILES}
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(OrderAnalyse-GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES_GUI}
    )
    qt_add_executable(OrderAnalyse-CLI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES_CLI}
    )


# Define target properties for Android with Qt 6 as:
#    set_property(TARGET OrderAnalyse-GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(OrderAnalyse-GUI SHARED
            ${PROJECT_SOURCES_GUI}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(OrderAnalyse-GUI
            ${PROJECT_SOURCES_GUI}
        )
        add_executable(OrderAnalyse-CLI
            ${PROJECT_SOURCES_CLI}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(OrderAnalyse-GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.

find_package(Qt6 COMPONENTS Core Widgets Charts REQUIRED)

target_link_libraries(OrderAnalyse-GUI PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.OrderAnalyse-GUI)
endif()
set_target_properties(OrderAnalyse-GUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
include_directories(include/order_analyse include/gui)
install(TARGETS OrderAnalyse-GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(OrderAnalyse-GUI)
endif()
