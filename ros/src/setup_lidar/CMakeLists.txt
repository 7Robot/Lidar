cmake_minimum_required(VERSION 2.8.3)
project(setup_lidar)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  laser_geometry
  message_filters
  roscpp
  sensor_msgs
  tf
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

## Declare a C++ executable
add_executable(pointcloud_publisher src/pointcloud_publisher.cpp)
add_executable(tf_broadcaster src/tf_broadcaster.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(pointcloud_publisher ${catkin_LIBRARIES})
target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})
