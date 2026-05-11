#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_srvs/srv/empty.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cstdio>

using namespace std::chrono_literals;
class patrolController : public rclcpp::Node
{
private:
  bool is_moving = true;
  double lin = 1.5;
  double ang = 1.0;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr m_publisher;
  rclcpp::TimerBase::SharedPtr m_timer;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr srv_stop;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr srv_continue;
  void timer_callback()
  {
    double lin = this->get_parameter("linear_speed").as_double();
    double ang = this->get_parameter("angular_speed").as_double();

    auto msg = geometry_msgs::msg::Twist();
    if (is_moving == true)
    {
      msg.angular.z = ang;
      msg.linear.x = lin;
    }
    else if (is_moving == false)
    {
      msg.angular.z = 0.0;
      msg.linear.x = 0.0;
    }
    m_publisher->publish(msg);
  }
  void handle_stop(const std::shared_ptr<std_srvs::srv::Empty::Request> request,
                   std::shared_ptr<std_srvs::srv::Empty::Response> response)
  {
    is_moving = false;
  }
  void handle_continue(const std::shared_ptr<std_srvs::srv::Empty::Request> request,
                       std::shared_ptr<std_srvs::srv::Empty::Response> response)
  {
    is_moving = true;
  }

public:
  patrolController() : Node("patrol_controller")
  {
    this->declare_parameter("linear_speed", 1.5);
    this->declare_parameter("angular_speed", 1.0);


    m_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    m_timer = this->create_wall_timer(100ms, std::bind(&patrolController::timer_callback, this));
    srv_stop = this->create_service<std_srvs::srv::Empty>("/stop", std::bind(&patrolController::handle_stop, this, std::placeholders::_1, std::placeholders::_2));
    srv_continue = this->create_service<std_srvs::srv::Empty>("/continue", std::bind(&patrolController::handle_continue, this, std::placeholders::_1, std::placeholders::_2));
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<patrolController>());
  rclcpp::shutdown();
  return 0;
}
