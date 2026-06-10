from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def static_tf_node(name, x, y, z, roll, pitch, yaw, parent, child):
    return Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name=name,
        arguments=[
            str(x), str(y), str(z),
            str(roll), str(pitch), str(yaw),
            parent,
            child
        ]
    )


def generate_launch_description():
    package_name = 'robot_tf_localization'
    package_share_dir = get_package_share_directory(package_name)

    ekf_config_path = os.path.join(package_share_dir, 'config', 'ekf.yaml')
    rviz_config_path = os.path.join(package_share_dir, 'config', 'robot_tf.rviz')

    static_transforms = [
        # base_footprint is on the ground.
        # base_link is 5 cm above the ground.
        static_tf_node(
            'base_footprint_to_base_link',
            0.0, 0.0, 0.05,
            0.0, 0.0, 0.0,
            'base_footprint',
            'base_link'
        ),

        # Robot length = 60 cm.
        # base_link is 10 cm from rear edge, so front edge is x = 0.50 m.
        # IMU is 5 cm behind the front edge => x = 0.45 m.
        # IMU height = 15 cm, base_link height = 5 cm => z = 0.10 m.
        static_tf_node(
            'base_link_to_imu_link',
            0.45, 0.0, 0.10,
            0.0, 0.0, 0.0,
            'base_link',
            'imu_link'
        ),

        # GPS is at the center of the robot.
        # Robot center is 30 cm from rear edge.
        # base_link is 10 cm from rear edge => x = 0.20 m.
        # GPS height = 30 cm, base_link height = 5 cm => z = 0.25 m.
        static_tf_node(
            'base_link_to_gps_link',
            0.20, 0.0, 0.25,
            0.0, 0.0, 0.0,
            'base_link',
            'gps_link'
        ),

        # Ultrasonic sensors height = 15 cm.
        # Relative to base_link height 5 cm => z = 0.10 m.
        # Width = 30 cm => left y = +0.15, right y = -0.15.
        # Front edge relative to base_link = +0.50 m.
        # Rear edge relative to base_link = -0.10 m.

        # Front Left: +45 deg
        static_tf_node(
            'base_link_to_ultrasonic1_link',
            0.50, 0.15, 0.10,
            0.0, 0.0, 0.785398,
            'base_link',
            'ultrasonic1_link'
        ),

        # Front Center: 0 deg
        static_tf_node(
            'base_link_to_ultrasonic2_link',
            0.50, 0.0, 0.10,
            0.0, 0.0, 0.0,
            'base_link',
            'ultrasonic2_link'
        ),

        # Front Right: -45 deg
        static_tf_node(
            'base_link_to_ultrasonic3_link',
            0.50, -0.15, 0.10,
            0.0, 0.0, -0.785398,
            'base_link',
            'ultrasonic3_link'
        ),

        # Rear Left: +135 deg
        static_tf_node(
            'base_link_to_ultrasonic4_link',
            -0.10, 0.15, 0.10,
            0.0, 0.0, 2.35619,
            'base_link',
            'ultrasonic4_link'
        ),

        # Rear Center: 180 deg
        static_tf_node(
            'base_link_to_ultrasonic5_link',
            -0.10, 0.0, 0.10,
            0.0, 0.0, 3.14159,
            'base_link',
            'ultrasonic5_link'
        ),

        # Rear Right: -135 deg
        static_tf_node(
            'base_link_to_ultrasonic6_link',
            -0.10, -0.15, 0.10,
            0.0, 0.0, -2.35619,
            'base_link',
            'ultrasonic6_link'
        ),
    ]

    ekf_node = Node(
        package='robot_localization',
        executable='ekf_node',
        name='ekf_filter_node',
        output='screen',
        parameters=[ekf_config_path],
        remappings=[
            ('odometry/filtered', '/odometry/local'),
        ]
    )

    orientation_analysis_node = Node(
        package=package_name,
        executable='orientation_analysis.py',
        name='orientation_analysis',
        output='screen'
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_config_path],
        output='screen'
    )

    return LaunchDescription(
        static_transforms
        + [
            ekf_node,
            orientation_analysis_node,
            rviz_node
        ]
    )