#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <chrono>
#include <functional>
#include <memory>


using namespace std::chrono_literals;
class PublisherA : public rclcpp::Node{
	private:
		rclcpp::TimerBase::SharedPtr m_timer;
		rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr m_publisher_a;
		size_t value_a;

		void timer_callback(){
		   auto msg = std_msgs::msg::Int32();
		   msg.data = value_a++ ;
		   m_publisher_a->publish(msg);
		   RCLCPP_INFO(this->get_logger(),"Publisher A publishing on topic_a: '%d'", msg.data);
			}
		
	public:
		PublisherA() : rclcpp::Node("publisher_a"), value_a(0){
			m_publisher_a = this->create_publisher<std_msgs::msg::Int32>("topic_a",10);
			m_timer = this->create_wall_timer(1s,std::bind(&PublisherA::timer_callback, this));
		}
};

int main(int argc , char ** argv){
	rclcpp::init(argc , argv);
	rclcpp::spin(std::make_shared<PublisherA>());
	rclcpp::shutdown();
	return 0;
}
