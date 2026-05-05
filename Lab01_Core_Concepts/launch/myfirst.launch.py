#!/usr/bin/env python3
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='lab01',
            executable='sensor_publisher.py',
            name='minimal_publisher',
            output='screen',
        ),

        Node(
            package='lab01',
            executable='subscriber.py',
            name='minimal_Subscriber',
            output='screen',
        )
    ])