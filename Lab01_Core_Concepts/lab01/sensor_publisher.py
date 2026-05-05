#!/usr/bin/env python3
import rclpy
import random
from rclpy.node import Node
from sensor_msgs.msg import Range

class Publisher(Node):
    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(Range, 'sensor/distance', 10)
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
    def timer_callback(self):
        msg = Range()
        msg.range = random.uniform(0.3,5)
        msg.header.stamp=self.get_clock().now().to_msg()
        msg.header.frame_id="abdelfattah_id"
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%.1f"' % msg.range)


def main(args=None):
    rclpy.init(args=args)

    rclpy.spin(Publisher())
    Publisher().destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()