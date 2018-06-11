cmake_minimum_required(VERSION 2.8)
project(lidar-mqtt-publish)

SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage -fPIC -std=c++0x -pthread ${CMAKE_CXX_FLAGS} -I/usr/local/include ")
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage -fPIC ${CMAKE_C_FLAGS} ")

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

find_package(OpenSSL REQUIRED)

message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")

include_directories(${OPENSSL_INCLUDE_DIR})
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

SET ( JSON_LIBRARY "jsoncpp" )
SET ( MQTT_CPP_LIBRARY "mqtt3cpp" )
SET ( MQTT_C_LIBRARY "mqtt3c" )

add_subdirectory(lib)

set(SYSTEM_LIBS ${THREAD_LIBS_SYSTEM} ${OPENSSL_LIB} ${OPENSSLCRYPTO_LIB} ${LIBS_SYSTEM})
set(COMMON_LIBS ${JSON_LIBRARY} ${MQTT_CPP_LIBRARY} ${MQTT_C_LIBRARY} ${LOG4CPP_LIBRARY_NAME})

add_executable(lidar-mqtt-publish lidar-mqtt-publish.cpp)
target_link_libraries (lidar-mqtt-publish ${COMMON_LIBS})

include_directories ("${PROJECT_SOURCE_DIR}/lib")