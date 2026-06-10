# Robot TF Localization - Lab 03

This package implements Lab 03: **TF Tree & Robot Localization** using ROS 2.

The goal is to build a complete TF tree for a mobile robot, run EKF-based localization using `robot_localization`, visualize the frames in RViz, and compare yaw orientation between IMU and filtered odometry.

---

## Package Name

```text
robot_tf_localization
````

---

## Main Features

* Publishes static transforms for the robot body and sensors.
* Runs `ekf_node` from `robot_localization`.
* Fuses wheel odometry and IMU data.
* Publishes filtered odometry on:

```text
/odometry/local
```

* Publishes dynamic transform:

```text
odom -> base_footprint
```

* Visualizes the TF tree and odometry in RViz.
* Runs an orientation analysis node to compare IMU yaw and odometry yaw.

---

## TF Tree

Expected TF tree:

```text
odom
└── base_footprint
    └── base_link
        ├── imu_link
        ├── gps_link
        ├── ultrasonic1_link
        ├── ultrasonic2_link
        ├── ultrasonic3_link
        ├── ultrasonic4_link
        ├── ultrasonic5_link
        └── ultrasonic6_link
```

---

## Static Transforms

### base_footprint -> base_link

`base_link` is 5 cm above the ground.

```text
x = 0.0
y = 0.0
z = 0.05
```

### base_link -> imu_link

The IMU is mounted near the front of the robot.

```text
x = 0.45
y = 0.0
z = 0.10
```

### base_link -> gps_link

The GPS is mounted at the center of the robot.

```text
x = 0.20
y = 0.0
z = 0.25
```

### Ultrasonic Sensors

The robot has six ultrasonic sensors:

```text
front_left
front_center
front_right
rear_left
rear_center
rear_right
```

They are published as:

```text
ultrasonic1_link
ultrasonic2_link
ultrasonic3_link
ultrasonic4_link
ultrasonic5_link
ultrasonic6_link
```

---

## EKF Configuration

The EKF node uses two input sources:

### Wheel Odometry

Topic:

```text
/odometry/wheel
```

Used values:

```text
linear velocity x
angular velocity z
```

### IMU

Topic:

```text
/imu/data
```

Used values:

```text
angular velocity z
```

The EKF output is published on:

```text
/odometry/local
```

The EKF also publishes:

```text
odom -> base_footprint
```

---

## QoS Notes

The rosbag sensor topics use sensor data QoS.

The `orientation_analysis.py` node uses:

```python
qos_profile_sensor_data
```

This makes it compatible with Best Effort sensor topics such as:

```text
/imu/data
```

---

## Orientation Analysis Node

The node subscribes to:

```text
/imu/data
/odometry/local
```

It extracts yaw from both messages and prints:

```text
IMU yaw
Odom yaw
Yaw error
```

This helps compare the IMU orientation with the filtered odometry orientation.

---

## Files

```text
robot_tf_localization
├── CMakeLists.txt
├── package.xml
├── config
│   ├── ekf.yaml
│   └── robot_tf.rviz
├── launch
│   └── robot_tf_localization.launch.py
└── scripts
    └── orientation_analysis.py
```

---

## How to Run

Build the package:

```bash
cd ~/ros2_ws
colcon build --packages-select robot_tf_localization
source install/setup.bash
```

Launch the system:

```bash
ros2 launch robot_tf_localization robot_tf_localization.launch.py
```

Play the rosbag in another terminal:

```bash
ros2 bag play <bag_name>
```

---

## Expected Result

After launching the system and playing the bag:

* RViz opens automatically.
* The TF tree is visible.
* Static sensor frames appear under `base_link`.
* EKF publishes `/odometry/local`.
* The dynamic transform `odom -> base_footprint` is available.
* The orientation analysis node prints yaw comparison logs.

---

## Author

AbdelFattah Moawed
