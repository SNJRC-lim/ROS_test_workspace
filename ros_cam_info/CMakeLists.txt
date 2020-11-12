cmake_minimum_required(VERSION 2.8.3)
project(ros_cam_info)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

find_package(catkin REQUIRED COMPONENTS
  roscpp
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

add_executable(cam_info_publisher src/cam_info_publisher.cpp)
target_link_libraries(cam_info_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(cam_info_subscriber src/cam_info_subscriber.cpp)
target_link_libraries(cam_info_subscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
