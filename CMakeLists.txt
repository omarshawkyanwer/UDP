cmake_minimum_required(VERSION 3.5.1)
project(UDP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
SET(CMAKE_BUILD_TYPE Debug)
set(SERVER_SOURCE_FILES server.cpp types.h types.cpp tcp_socket.cpp tcp_socket.h transmission.h protocols/selective_repeat.cpp protocols/selective_repeat.h main.cpp protocols/congestion_control.h protocols/additive_multiplicative.cpp protocols/additive_multiplicative.h protocols/slow_starter.h protocols/slow_starter.cpp)
set(CLIENT_SOURCE_FILES client.cpp types.h types.cpp main.cpp protocols/congestion_control.h protocols/additive_multiplicative.cpp protocols/additive_multiplicative.h protocols/slow_starter.h protocols/slow_starter.cpp)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

add_executable(server ${SERVER_SOURCE_FILES})
add_executable(client ${CLIENT_SOURCE_FILES})

if (Boost_FOUND)
    target_link_libraries(server ${Boost_LIBRARIES})
    target_link_libraries(client ${Boost_LIBRARIES})
endif ()
