cmake_minimum_required(VERSION 3.26)
project(FuzzToJson)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core Sql
        REQUIRED)

add_library(solarsystems STATIC
        ../../SolarSystem.cpp
        ../../SolarSystem.h
        ../../SolarSystemsMap.cpp
        ../../SolarSystemsMap.h)

set_target_properties(solarsystems PROPERTIES
        AUTOMOC OFF
        AUTORCC OFF
        AUTOUIC OFF
)

add_executable(FuzzToJson main.cpp
        DbManager.cpp
        DbManager.h
)
target_link_libraries(FuzzToJson
        Qt::Core Qt::Sql solarsystems
)

