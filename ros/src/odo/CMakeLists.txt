cmake_minimum_required(VERSION 2.8.3)
project(odo)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  nav_msgs
  tf
  comm
)

catkin_package()
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
add_executable(odo src/odo.cpp)

## Add dependencies
add_dependencies(odo comm_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(odo ${catkin_LIBRARIES})
