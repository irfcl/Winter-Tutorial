cmake_minimum_required(VERSION 3.5)
project(cpp_tf2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(
  include
)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

add_executable(frame_broadcaster src/frame_broadcaster.cpp)
ament_target_dependencies(frame_broadcaster 
  rclcpp  
  std_msgs
  geometry_msgs
  tf2_ros
)
add_executable(frame_listener src/frame_listener.cpp)
ament_target_dependencies(frame_listener 
  rclcpp  
  std_msgs
  geometry_msgs
  tf2_ros
)
add_executable(point_transformer src/point_transformer.cpp)
ament_target_dependencies(point_transformer 
  rclcpp  
  std_msgs
  geometry_msgs
  tf2_ros
  tf2_geometry_msgs
)

install(TARGETS
  frame_broadcaster
  frame_listener
  point_transformer
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY 
  launch/
  # rviz/
  DESTINATION share/${PROJECT_NAME}/launch
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
