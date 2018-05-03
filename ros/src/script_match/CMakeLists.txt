cmake_minimum_required(VERSION 2.8.3)
project(script_match)

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

## Declare a C++ executable
add_executable(script_match_gr src/main_gr.cpp)

add_dependencies(script_match_gr comm_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(script_match_gr ${catkin_LIBRARIES})
