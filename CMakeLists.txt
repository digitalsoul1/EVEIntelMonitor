cmake_minimum_required(VERSION 3.24)
set(APPLICATION_NAME "EVEIntelMonitor")
project(${APPLICATION_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CXX_CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 COMPONENTS
        Core HttpServer Quick Concurrent
        REQUIRED)

qt_policy(SET QTP0001 NEW)
SET(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.7)

set(HEADER_FILES
        SSO/Auth.h
        RoutePlanner/RoutePlanner.h
        RoutePlanner/ShortestRoute_BFS.h
        RoutePlanner/SolarSystem.h
        RoutePlanner/SolarSystemsMap.h
        SSO/Manager.h
        ConfigBackend/ConfigBackend.h
        SSO/User.h
        Generic/Result.h
        Controller.h
        SSO/CharacterLocation.h
        Intel/Monitor.h
        Intel/IntelChannel.h
        Intel/Intel.h
        Intel/Parser.h
        Intel/QQueueSizeConstrained.h
        Utility/FileDownloader.h
        IntelChannelsListView.h
        EveScout/TurnurTheraConnections.h
        Intel/PersistentSettings.h
        QTimerPool.h
)

set(SOURCE_FILES
        SSO/Auth.cpp
        RoutePlanner/RoutePlanner.cpp
        RoutePlanner/ShortestRoute_BFS.cpp
        RoutePlanner/SolarSystem.cpp
        RoutePlanner/SolarSystemsMap.cpp
        main.cpp
        SSO/Manager.cpp
        ConfigBackend/ConfigBackend.cpp
        SSO/User.cpp
        Controller.cpp
        SSO/CharacterLocation.cpp
        Intel/Monitor.cpp
        Intel/IntelChannel.cpp
        Intel/Intel.cpp
        Intel/Parser.cpp
        Utility/FileDownloader.cpp
        IntelChannelsListView.cpp
        EveScout/TurnurTheraConnections.cpp
        Intel/PersistentSettings.cpp
        QTimerPool.cpp
)

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE app-icon-macos.icns)
    set(APP_ICON_MACOS "${CMAKE_SOURCE_DIR}/Assets/Images/app-icon-macos.icns")
    set_source_files_properties(${APP_ICON_MACOS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    qt_add_executable(${APPLICATION_NAME}
            MACOSX_BUNDLE
            ${APP_ICON_MACOS}
            ${HEADER_FILES}
            ${SOURCE_FILES}
            resources.qrc
    )
else ()
    qt_add_executable(${APPLICATION_NAME}
            ${HEADER_FILES}
            ${SOURCE_FILES}
            resources.qrc
    )
endif ()


qt_add_qml_module(
        ${APPLICATION_NAME}
        URI ${APPLICATION_NAME}
        VERSION 0.5
        QML_FILES
        QML/MainWindow.qml
        QML/ClickMenuButton.qml
        QML/AboutDialog.qml
        QML/DeleteCharacterDialog.qml
        QML/DialogFrame.qml
        QML/SettingsDialog.qml
        SOURCES Controller.h Controller.cpp
        SOURCES IntelTableView.h IntelTableView.cpp
        SOURCES MainWindowView.cpp MainWindowView.h
)

target_compile_options(${APPLICATION_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(${APPLICATION_NAME} PRIVATE Qt6::Core Qt6::HttpServer Qt6::Quick Qt6::Concurrent)

if (APPLE)
    find_program(MACDEPLOYQT_BINARY NAMES macdeployqt)

    if (MACDEPLOYQT_BINARY STREQUAL "MACDEPLOYQT_BINARY-NOTFOUND")
        message(FATAL_ERROR "Could not find the macdeployqt tool. The application bundle will not be portable.")
    else ()
        add_custom_command(TARGET ${APPLICATION_NAME} POST_BUILD
                COMMAND ${MACDEPLOYQT_BINARY} ${APPLICATION_NAME}.app -qmldir=${CMAKE_SOURCE_DIR}/QML
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif ()
endif ()
