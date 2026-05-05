
# Lab 1: Core Concepts (Topics & Messages)

This project contains **all 5 tasks in one ROS 2 package**.

I used a **single mixed package** that includes both:

- **C++ nodes**
- **Python nodes**

The package uses **CMake** to build the C++ executables and also install/run the Python scripts from the same package.

---

## Package Summary

This package implements the following tasks:

1. **CPU Temperature Publisher**  
   - Language: **C++**
   - Topic: `/cpu_temp`

2. **Obstacle Detection Logic**  
   - Language: **Python**
   - Topics: `/sensor/distance`, `/cmd/stop`

3. **Command Velocity Limiter**  
   - Language: **C++**
   - Topics: `/cmd_vel`, `/cmd_vel_limited`

4. **Odometry Path Publisher**  
   - Language: **Python**
   - Topic: `/odom`

5. **IMU CSV Playback Node**  
   - Language: **Python**
   - Topic: `/imu/data`

---

## Package Structure

Example structure:

```bash
lab1_core_concepts/
├── CMakeLists.txt
├── package.xml
├── README.md
├── .gitignore
├── data
│   └── imu_data.csv
├── include
│   └── lab01
├── lab01
│   ├── __init__.py
│   ├── IMU_CSV_Playback_Node.py
│   ├── sensor_publisher.py
│   └── subscriber.py
├── launch
│   └── myfirst.launch.py
└── src
    ├── task1.cpp
    ├── task3.cpp
    └── task4.cpp
```

---

## Dependencies

Make sure these dependencies are included in `package.xml`:

- `rclcpp`
- `rclpy`
- `std_msgs`
- `geometry_msgs`
- `nav_msgs`
- `sensor_msgs`


---


## Notes

- This lab is implemented as **one package for all tasks** instead of creating separate packages.
- The package contains a mix of **C++ and Python** nodes.
- **CMake** is used to build the C++ nodes and install the Python executables in the same ROS 2 package.
- The IMU publisher reads data from `imu_data.csv` and loops back to the beginning when it reaches the end of the file.

---

## Verification

Each task was tested using `ros2 topic echo` on its required topic:

- `/cpu_temp`
- `/sensor/distance`
- `/cmd/stop`
- `/cmd_vel_limited`
- `/odom`
- `/imu/data`

---

## Author
**Abdelfattah**