cmake_minimum_required(VERSION 2.6)

set(NAME battery_info_mqtt)

project(${NAME})

if(WIN32)
    set(ROBOTINOAPI2_DIR "$ENV{ProgramFiles}/REC GmbH/robotino/api2")
else(WIN32)
    set(ROBOTINOAPI2_DIR "/opt/robotino")
endif(WIN32)

set(CMAKE_MODULE_PATH ${ROBOTINOAPI2_DIR}/cmake)
set(PAHO_DIR "/home/robotino/pahoCPP")
set(PAHO_DIR_C "/home/robotino/pahoC")

find_package(RobotinoAPI2 REQUIRED)

copy_robotinoapi2_dlls("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB SRCS *.cpp)
file(GLOB HEADER *.h)

add_executable(
    ${NAME}
    ${SRCS}
    ${HEADER}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PAHO_DIR}/include
)

target_link_libraries(
    ${NAME}
    ${REC_ROBOTINO_API2_LIBRARY}
    ${PAHO_DIR}/build/src/libpaho-mqttpp3.so
    ${PAHO_DIR_C}/build/src/libpaho-mqtt3as.so
)

source_group("Source Files" FILES ${SRCS} ${HEADER})

# Add the following line to specify the C++11 standard
target_compile_features(${NAME} PRIVATE cxx_std_11)

