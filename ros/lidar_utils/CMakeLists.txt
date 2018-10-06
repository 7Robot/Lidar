cmake_minimum_required(VERSION 2.8.3)
project(lidar_utils)

find_package(catkin REQUIRED COMPONENTS
  laser_geometry
  roscpp
  sensor_msgs
  tf
)

catkin_package()

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(dev_lidar src/dev.cpp)

target_link_libraries(dev_lidar	${catkin_LIBRARIES})
