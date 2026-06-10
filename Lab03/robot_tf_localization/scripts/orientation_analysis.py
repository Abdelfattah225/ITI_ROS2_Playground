#!/usr/bin/env python3

import math

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry


def quaternion_to_yaw(q):
    """
    Convert quaternion orientation to yaw angle in radians.
    """
    siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
    cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
    return math.atan2(siny_cosp, cosy_cosp)


def normalize_angle(angle):
    """
    Normalize angle to range [-pi, pi].
    """
    while angle > math.pi:
        angle -= 2.0 * math.pi

    while angle < -math.pi:
        angle += 2.0 * math.pi

    return angle


class OrientationAnalysis(Node):
    def __init__(self):
        super().__init__('orientation_analysis')

        self.imu_yaw = None
        self.odom_yaw = None

        self.imu_subscription = self.create_subscription(
            Imu,
            '/imu/data',
            self.imu_callback,
            qos_profile_sensor_data
        )

        self.odom_subscription = self.create_subscription(
            Odometry,
            '/odometry/local',
            self.odom_callback,
            qos_profile_sensor_data
        )

        self.timer = self.create_timer(1.0, self.print_analysis)

        self.get_logger().info('Orientation Analysis node started.')

    def imu_callback(self, msg):
        self.imu_yaw = quaternion_to_yaw(msg.orientation)

    def odom_callback(self, msg):
        self.odom_yaw = quaternion_to_yaw(msg.pose.pose.orientation)

    def print_analysis(self):
        if self.imu_yaw is None or self.odom_yaw is None:
            self.get_logger().info('Waiting for /imu/data and /odometry/local...')
            return

        yaw_error = normalize_angle(self.odom_yaw - self.imu_yaw)

        self.get_logger().info(
            f'IMU yaw: {math.degrees(self.imu_yaw):.2f} deg | '
            f'Odom yaw: {math.degrees(self.odom_yaw):.2f} deg | '
            f'Error: {math.degrees(yaw_error):.2f} deg'
        )


def main(args=None):
    rclpy.init(args=args)

    node = OrientationAnalysis()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()