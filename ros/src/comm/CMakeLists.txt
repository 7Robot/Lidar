cmake_minimum_required(VERSION 2.8.3)
project(comm)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  genmsg
  std_msgs
  message_generation
  serial
)

################################################
## Declare ROS messages, services and actions ##
################################################
add_service_files(
  FILES
  Comm.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS std_msgs
)
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_executable(comm src/comm.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(comm ${catkin_LIBRARIES})
