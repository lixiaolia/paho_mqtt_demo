PROJECT(PAHO_DEMO LANGUAGES CXX C)
cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
SET(PAHO_CPP_LIB paho-mqtt3c)

add_executable(mqtt_sub mqtt_sub.cpp)
add_executable(mqtt_pub mqtt_pub.cpp)
target_link_libraries(mqtt_sub ${PAHO_CPP_LIB})
target_link_libraries(mqtt_pub ${PAHO_CPP_LIB})
