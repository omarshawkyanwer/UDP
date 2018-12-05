cmake_minimum_required(VERSION 3.5)
project(Reliable_Data_Transfer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
set(SOURCES main.cpp Types.h chuncking.cpp)
find_package(Boost COMPONENTS system filesystem thread REQUIRED)
if (Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif ()
#target_link_libraries(main.cpp ${Boost_LIBRARIES})

add_executable(Reliable_Data_Transfer ${SOURCES})

if (Boost_FOUND)

    target_link_libraries(Reliable_Data_Transfer ${Boost_LIBRARIES})

endif ()