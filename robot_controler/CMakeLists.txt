cmake_minimum_required(VERSION 2.8.3)
project(robot_controler)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)
find_package(OpenCV REQUIRED COMPONENTS)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES opencv_color_detec
#  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


link_directories(${OpenCV_LIB_DIRS})

add_executable(main_controler src/main_controler.cpp)
target_link_libraries(main_controler ${catkin_LIBRARIES})

add_executable(cam_info_publisher src/cam_info_publisher.cpp)
target_link_libraries(cam_info_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(cam_info_subscriber src/cam_info_subscriber.cpp)
target_link_libraries(cam_info_subscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
