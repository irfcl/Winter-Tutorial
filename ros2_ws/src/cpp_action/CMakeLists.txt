cmake_minimum_required(VERSION 3.8)
project(cpp_action)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(std_msgs REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(integrated_node
  src/counter_client.cpp
  src/counter_server.cpp
  src/main.cpp
)

add_executable(integrated_node_1
  src/counter_client.cpp
  src/counter_server.cpp
  src/main1.cpp
)

target_include_directories(integrated_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(integrated_node_1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(integrated_node
  rclcpp
  rclcpp_action
  example_interfaces
  std_msgs
)

ament_target_dependencies(integrated_node_1
  rclcpp
  rclcpp_action
  example_interfaces
  std_msgs
)

add_executable(thread_example src/thread_example.cpp)
find_package(Threads REQUIRED)
target_link_libraries(thread_example Threads::Threads)

add_executable(thread_example_1 src/thread_example_1.cpp)
find_package(Threads REQUIRED)
target_link_libraries(thread_example_1 Threads::Threads)

install(TARGETS
  integrated_node
  thread_example
  integrated_node_1
  thread_example_1
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
