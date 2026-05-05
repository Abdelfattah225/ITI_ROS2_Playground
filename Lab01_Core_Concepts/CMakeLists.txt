cmake_minimum_required(VERSION 3.8)
project(lab01)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
#cfind_package(launch_ros REQUIRED)
find_package(rclpy REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
add_executable(task1 src/task1.cpp)
add_executable(task3 src/task3.cpp)
add_executable(task4 src/task4.cpp)


target_include_directories(task1 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(task1 PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
#task 1 and 2
ament_target_dependencies(
  task1
  "rclpy"
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "sensor_msgs"
)
#task3 
ament_target_dependencies(
  task3
  "rclcpp"
  "geometry_msgs"
)

#task4 
ament_target_dependencies(
  task4
  "rclcpp"
  "nav_msgs"
)

install(TARGETS task1 task3 task4
  DESTINATION lib/${PROJECT_NAME})

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


ament_python_install_package(${PROJECT_NAME})

# Install Python scripts as runnable nodes
install(PROGRAMS
  lab01/sensor_publisher.py
  lab01/subscriber.py
  lab01/IMU_CSV_Playback_Node.py
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch)

install(
  DIRECTORY data/
  DESTINATION share/${PROJECT_NAME}/data
)  
ament_package()

