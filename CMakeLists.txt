cmake_minimum_required(VERSION 3.8)
project(OpticalExperiment)

set(CMAKE_CXX_STANDARD 17)

# включаем поиск библиотек с использованием find_package()
# через REQUIRED указываем, что библиотеки обязательны
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)

file(GLOB helloworld_SRC
        "*.h"
        "*.hpp"
        "*.cpp"
        "*.qrc"
        "deviceconfigs/*"
        "ui/*"
        "utility/*",
        "command/*"
        )

set(SOURCE_FILES main.cpp)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(OpticalExperiment ${helloworld_SRC})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deviceconfigs)

# указываем, какие библиотеки необходимо подключить
target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Charts)