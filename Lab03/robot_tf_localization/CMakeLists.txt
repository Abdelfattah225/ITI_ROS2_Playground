cmake_minimum_required(VERSION 3.10)
project(robot_tf_localization)

find_package(ament_cmake REQUIRED)

install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

install(
  PROGRAMS
    scripts/orientation_analysis.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()