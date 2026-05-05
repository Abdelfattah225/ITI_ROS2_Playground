#!/usr/bin/env python3
import rclpy
import random
from rclpy.node import Node
from sensor_msgs.msg import Range
from std_msgs.msg import Bool


class Subscriber(Node):
    def __init__(self):
        super().__init__('minimal_Subscriber')
        self.publisher_ = self.create_publisher(Bool, '/cmd/stop', 10)
        self.subscriper_ = self.create_subscription(Range,'sensor/distance', self.timer_callback,10)
    def timer_callback(self,msg):
        control_msg = Bool()
        if msg.range < 2.0:
            control_msg.data = True
            self.get_logger().info('data: ture ')

        else:
            control_msg.data = False
            self.get_logger().info('data: false ')
        self.publisher_.publish(control_msg)




def main(args=None):
    rclpy.init(args=args)

    sub = Subscriber()

    rclpy.spin(sub)
    sub.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()



