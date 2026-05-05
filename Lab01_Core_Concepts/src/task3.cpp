#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
#include <functional>
#include <memory>


using std::placeholders::_1;
class CommandVelocityLimiter : public rclcpp::Node{
    private:
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr m_subscriber;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr m_publisher;
        void subscription_handler(const geometry_msgs::msg::Twist::SharedPtr msg ){
            double linearSpeed = msg->linear.x;
            double angularSpeed = msg->angular.z;
            RCLCPP_INFO(this->get_logger(), "Received cmd_vel");
            if(msg->linear.x > 1.0)
            {
                RCLCPP_WARN(this->get_logger(),"Limiting linear speed to 1 m/s, Speed was: %f m/s.",linearSpeed);
                linearSpeed = 1.0;
            }
            else if(msg->linear.x < -1.0)
            {
              RCLCPP_WARN(this->get_logger(),"Limiting linear speed to -1 m/s, Speed was: %f m/s.",linearSpeed);
                linearSpeed = -1.0;   
            }
            if(msg->angular.z > 1.5)
            {
                RCLCPP_WARN(this->get_logger(),"Limiting angluar speed to 1.5 rad/s, Speed was: %f rad/s.",angularSpeed);
                angularSpeed = 1.5;
            }
            if(msg->angular.z < -1.5)
            {
                RCLCPP_WARN(this->get_logger(),"Limiting angluar speed to -1.5 rad/s, Speed was: %f rad/s.",angularSpeed);
                angularSpeed = -1.5;
            }
            // publish values
            auto publisher_msg = geometry_msgs::msg::Twist();
            publisher_msg.linear.x=linearSpeed;
            publisher_msg.angular.z=angularSpeed;
            m_publisher->publish(publisher_msg);
        }
    public:
        CommandVelocityLimiter() : Node("cmdvellimiter_node"){
            m_subscriber = this->create_subscription<geometry_msgs::msg::Twist>("/cmd_vel",10,std::bind(&CommandVelocityLimiter::subscription_handler,this,_1));
            m_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel_limited",10);
        }
};


int main(int argc, char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<CommandVelocityLimiter>());
    rclcpp::shutdown();
    return 0;
}
