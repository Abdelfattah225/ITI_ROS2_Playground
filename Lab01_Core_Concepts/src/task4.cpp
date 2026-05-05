#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <functional>
#include <chrono>
#include <memory>


using namespace std::chrono_literals;

class OdometryPathPublisher : public rclcpp::Node{
    private:
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr m_publisher;
        rclcpp::TimerBase::SharedPtr m_timer;
        void timer_callback(){
            auto msg = nav_msgs::msg::Odometry();
            msg.pose.pose.position.x += 0.1;
            msg.twist.twist.linear.x = 0.1;
            msg.child_frame_id = "base_link";
            msg.header.frame_id ="odom";
            msg.header.stamp = this->get_clock()->now();
            msg.pose.pose.orientation.w = 1.0;
            m_publisher->publish(msg);

        }
    public:
        OdometryPathPublisher() : Node("odem_node"){
            m_publisher = this->create_publisher<nav_msgs::msg::Odometry>("/odom",10);
            m_timer = this->create_wall_timer(100ms,std::bind(&OdometryPathPublisher::timer_callback,this));
        }

};


int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdometryPathPublisher>());
    rclcpp::shutdown();
    return 0;
}

