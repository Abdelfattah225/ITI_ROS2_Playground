cmake_minimum_required(VERSION 3.10)
project(simple_turtle_patrol)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# Generate interfaces
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
  DEPENDENCIES geometry_msgs
)

# This is needed to use the generated messages in the same package
rosidl_get_typesupport_target(cpp_typesupport_target
  ${PROJECT_NAME} rosidl_typesupport_cpp)

# Create executables
add_executable(patrol_controller src/patrol_controller.cpp)
add_executable(status_publisher src/status_publisher.cpp)

# Include directories
target_include_directories(patrol_controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(status_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Set compile features
target_compile_features(patrol_controller PUBLIC c_std_99 cxx_std_17)
target_compile_features(status_publisher PUBLIC c_std_99 cxx_std_17)

# Set dependencies for patrol_controller
ament_target_dependencies(patrol_controller
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "turtlesim"
  "std_srvs"
)

# Set dependencies for status_publisher
ament_target_dependencies(status_publisher
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "turtlesim"
  "std_srvs"
)

# Link the custom message to status_publisher
target_link_libraries(status_publisher "${cpp_typesupport_target}")

# Install targets
install(TARGETS patrol_controller status_publisher
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  params
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_dependencies(rosidl_default_runtime)

ament_package()