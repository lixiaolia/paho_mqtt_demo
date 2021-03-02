PROJECT(PAHO_DEMO LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

OPTION (WITH_SSL "Build With SSL" OFF)
CONFIGURE_FILE (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h")

add_executable(mqtt_sub mqtt_sub.cpp)
add_executable(mqtt_pub mqtt_pub.cpp)

if(WITH_SSL)
target_link_libraries(mqtt_sub paho-mqtt3cs)
target_link_libraries(mqtt_pub paho-mqtt3cs)
else()
target_link_libraries(mqtt_sub paho-mqtt3c)
target_link_libraries(mqtt_pub paho-mqtt3c)
endif()