cmake_minimum_required(VERSION 3.6)
project(lidar-mqtt-publish)

SET(CMAKE_CXX_FLAGS "-g -O3 -Wall -fprofile-arcs -ftest-coverage -fPIC -std=c++0x -pthread ${CMAKE_CXX_FLAGS} -I/usr/local/include ")
SET(CMAKE_C_FLAGS "-g -O3 -Wall -W -fprofile-arcs -ftest-coverage -fPIC ${CMAKE_C_FLAGS} ")

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

find_package(OpenSSL REQUIRED)

message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

include_directories(${OPENSSL_INCLUDE_DIR})
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

SET ( JSON_LIBRARY "jsoncpp" )
SET ( MQTT_CPP_LIBRARY "mqtt3cpp" )
SET ( MQTT_C_LIBRARY "mqtt3c" )
SET ( LIDAR_LIBRARY "ydlidar_driver" )

add_subdirectory(lib)
add_subdirectory(sdk)

set(SYSTEM_LIBS ${THREAD_LIBS_SYSTEM} ${OPENSSL_LIB} ${OPENSSLCRYPTO_LIB} ${LIBS_SYSTEM})
set(COMMON_LIBS ${JSON_LIBRARY} ${MQTT_CPP_LIBRARY} ${MQTT_C_LIBRARY} ${LIDAR_LIBRARY})

add_executable(lidar-mqtt-publish lidar-mqtt-publish.cpp)
target_link_libraries (lidar-mqtt-publish ${COMMON_LIBS})

include_directories ("${PROJECT_SOURCE_DIR}/lib")
include_directories ("${PROJECT_SOURCE_DIR}/sdk/include")