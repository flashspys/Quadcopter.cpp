cmake_minimum_required(VERSION 3.6)
project(Quadrocopter)

set(CMAKE_CXX_STANDARD 14)

#include lrt and pigpio for Pigpio
#set c++ standard explicitly
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lrt -lpigpio -O1")

#include eigen
INCLUDE_DIRECTORIES ( "/usr/local/include/eigen3" )

#include boost with system(for asio)
set(BOOST_ROOT /root/boost/boost_1_63_0)
set(BOOST_LIBRARYDIR root/boost/boost_1_63_0/libs)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost COMPONENTS system thread date_time)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(SOURCE_FILES main.cpp Motor.cpp Motor.hpp Quadcopter.cpp Quadcopter.hpp Singleton.cpp LSM6DS33.cpp LSM6DS33.hpp I2CDevice.cpp I2CDevice.hpp pigpio.h LIS3MDL.cpp LIS3MDL.hpp UDPServer.cpp UDPServer.hpp vector.hpp MinIMU9.cpp MinIMU9.hpp IMU.hpp PID.cpp PID.hpp IMUCalculation.hpp IMUCalculation.cpp Log.cpp Log.hpp)
    add_executable(Quadrocopter ${SOURCE_FILES})
    target_link_libraries(Quadrocopter ${Boost_LIBRARIES})
endif()
