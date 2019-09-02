cmake_minimum_required(VERSION 3.1.0)
# Set directory for arm compiler, this needs to be before project()
set(CMAKE_C_COMPILER /home/sami/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /home/sami/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

project(AV_Control)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


# Add correct path for cross-compiler
find_package(Qt5 COMPONENTS Core PATHS /home/sami/raspi/qtbase/lib/cmake/Qt5/)
get_target_property(QtCore_location Qt5::Core LOCATION)

add_executable(AV_Control
    main.cpp pjlink.cpp
)

target_link_libraries(AV_Control Qt5::Core)