#include "rclcpp/rclcpp.hpp"
#include "simple_turtle_patrol/msg/robot_status.hpp"
#include "turtlesim/msg/pose.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <cmath>

class StatusPublisher : public rclcpp::Node {
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber_;
    rclcpp::Publisher<simple_turtle_patrol::msg::RobotStatus>::SharedPtr status_publisher_;
    size_t counter_ = 32;

    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg) {
        auto status_msg = simple_turtle_patrol::msg::RobotStatus();

        // Set position data
        status_msg.pose.x = msg->x;
        status_msg.pose.y = msg->y;
        status_msg.pose.theta = msg->theta;

        // Determine movement status
        if (std::abs(msg->linear_velocity) < 0.01 && std::abs(msg->angular_velocity) < 0.01) {
            status_msg.state = "stopped";
        } else {
            status_msg.state = "running";
        }

        // Set additional data
        status_msg.temperature = counter_++;
        status_msg.lap_count = msg->theta / (2.0 * M_PI);;

        // Publish the message
        status_publisher_->publish(status_msg);
    }

public:
    StatusPublisher() : Node("status_publisher") {
        // Create publisher for robot status
        status_publisher_ = this->create_publisher<simple_turtle_patrol::msg::RobotStatus>("/robot/status", 10);

        // Create subscriber for turtle pose
        pose_subscriber_ = this->create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose", 10,
            std::bind(&StatusPublisher::pose_callback, this, std::placeholders::_1));
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatusPublisher>());
    rclcpp::shutdown();
    return 0;
}