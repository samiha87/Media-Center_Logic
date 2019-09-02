cmake_minimum_required(VERSION 3.1.0)
# Set directory for arm compiler, this needs to be before project()
set(CMAKE_C_COMPILER /home/sami/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /home/sami/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

include_directories(${Qt5Core_INCLUDE_DIRS} ${QtNetwork_INCLUDE_DIRS})
add_definitions(${Qt5Core_DEFINITIONS} ${QtNetwork_DEFINITIONS})

project(AV_Control)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# Add correct path for cross-compiler
find_package(Qt5 COMPONENTS Core Network PATHS /home/sami/raspi/qt5pi/lib/cmake/Qt5/)

get_target_property(QtCore_location Qt5::Core LOCATION)
get_target_property(QtCore_location Qt5::Network LOCATION)

#list(APPEND library_sources ${moc_sources} )

add_executable(AV_Control
    main.cpp 
    pjlink.cpp 
    hardwareadapter.cpp 
    logic.cpp 
    tcpsocket.cpp
)

#qt5_use_modules(AV_Control Core Network)

target_link_libraries(AV_Control Qt5::Core Qt5::Network)
