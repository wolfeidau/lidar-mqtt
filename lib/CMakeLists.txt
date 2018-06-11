cmake_minimum_required(VERSION 2.8)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

add_library(${JSON_LIBRARY} jsoncpp.cpp)

add_library(${MQTT_CPP_LIBRARY} async_client.cpp client.cpp connect_options.cpp disconnect_options.cpp
                                iclient_persistence.cpp message.cpp response_options.cpp
                                ssl_options.cpp string_collection.cpp token.cpp topic.cpp will_options.cpp
                        )

add_library(${MQTT_C_LIBRARY} MQTTAsync.c Clients.c Heap.c LinkedList.c Log.c Messages.c
                              MQTTClient.c MQTTPacket.c MQTTPacketOut.c MQTTPersistence.c
                              MQTTPersistenceDefault.c MQTTProtocolClient.c MQTTProtocolOut.c
                              MQTTVersion.c SocketBuffer.c Socket.c SSLSocket.c StackTrace.c
                              Thread.c Tree.c utf-8.c
                        )