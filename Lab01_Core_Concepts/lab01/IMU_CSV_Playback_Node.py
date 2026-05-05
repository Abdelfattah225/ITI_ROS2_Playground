#!/usr/bin/env python3
import rclpy
import csv
import os
from rclpy.node import Node
from sensor_msgs.msg import Imu
from ament_index_python.packages import get_package_share_directory
import os
class ImuPlayback(Node):
    def __init__(self):
        super().__init__('imu_playback')

        self.publisher_ = self.create_publisher(Imu, '/imu/data', 10)
        self.timer_ = self.create_timer(0.1, self.timer_callback)

        # Load CSV ONCE
       
        csv_path = os.path.join(
            get_package_share_directory('lab01'),
            'data',
            'imu_data.csv'
        )

        self.data = []
        with open(csv_path, 'r') as file:
            reader = csv.DictReader(file)
            for row in reader:
                self.data.append(row)

        self.counter = 0

        self.get_logger().info(f"Loaded {len(self.data)} rows")

    def timer_callback(self):
        if not self.data:
            return

        msg = Imu()

        row = self.data[self.counter]

        # Timestamp
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = "base_link"

        # Orientation (check naming carefully!)
        msg.orientation.x = float(row['orient_x'])
        msg.orientation.y = float(row['orient_y'])
        msg.orientation.z = float(row['orient_z'])
        msg.orientation.w = float(row['orient_w'])

        # Angular velocity
        msg.angular_velocity.x = float(row['ang_x'])
        msg.angular_velocity.y = float(row['ang_y'])
        msg.angular_velocity.z = float(row['ang_z'])

        # Linear acceleration
        msg.linear_acceleration.x = float(row['acc_x'])
        msg.linear_acceleration.y = float(row['acc_y'])
        msg.linear_acceleration.z = float(row['acc_z'])

        # Publish
        self.publisher_.publish(msg)

        # Loop index
        self.counter = (self.counter + 1) % len(self.data)

        self.get_logger().info(f"Publishing row {self.counter}")


def main(args=None):
    rclpy.init(args=args)

    node = ImuPlayback()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()