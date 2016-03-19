cmake_minimum_required(VERSION 2.8.3)
project(publish_ICL_datasets)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  image_transport
  cv_bridge
)

catkin_package(
)

###########
## Build ##
###########

include_directories( include
  ${catkin_INCLUDE_DIRS}
)

add_executable(publisher src/publisher.cpp src/VaFRIC/VaFRIC.cpp)
target_link_libraries(publisher ${catkin_LIBRARIES})
add_dependencies(publisher publish_ICL_datasets_generate_message_cpp)

add_executable(tester src/tester.cpp)
target_link_libraries(tester ${catkin_LIBRARIES})
add_dependencies(tester publish_ICL_datasets_generate_message_cpp)
