#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

#include <chrono>
#include <functional>
#include <memory>

using namespace std::chrono_literals;

class PublisherB : public rclcpp::Node{
private:
	size_t value_b;
	rclcpp::TimerBase::SharedPtr m_timer;
	rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr m_publisher_b;
	
	void timer_callback(){
	 auto msg = std_msgs::msg::Int32();
	 msg.data = value_b++;
	m_publisher_b->publish(msg);
	RCLCPP_INFO(this->get_logger(),"Publisher B publishing on topic_b: '%d'",msg.data);
	}
public:
	PublisherB() : Node("publisher_b"), value_b(48){
	m_publisher_b = this->create_publisher<std_msgs::msg::Int32>("topic_b",10);
	m_timer = this->create_wall_timer(1s,std::bind(&PublisherB::timer_callback,this));
	
	}
};

int main(int argc , char **argv){
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<PublisherB>());
	rclcpp::shutdown();
	
	return 0;
}
