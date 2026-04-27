#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

#include <chrono>
#include <functional>
#include <memory>

using std::placeholders::_1;

class SumSubscriber : public rclcpp::Node{
private:
	rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr m_subscriber_a;
        rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr m_subscriber_b;
	rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr m_publisher_sum;
	size_t value_a;
	size_t value_b;
	bool has_a;
	bool has_b;
	size_t compute_value;

	void callback_a(const std_msgs::msg::Int32::SharedPtr  msg) {
	value_a = msg->data;
	has_a=true;
	this->compute_and_publish_sum();
	}
 	void callback_b(const std_msgs::msg::Int32::SharedPtr msg) { 
	value_b = msg->data;
	has_b=true;
        this->compute_and_publish_sum();
        }
	void compute_and_publish_sum(){
		if(has_a==true && has_b==true){
			auto msg = std_msgs::msg::Int32();
			compute_value = value_b + value_a;
			msg.data = compute_value;
			RCLCPP_INFO(this->get_logger(),"a=%ld, b=%ld, sum=%ld",value_a, value_b, compute_value);
			m_publisher_sum->publish(msg);	
			has_a = has_b = false;
		}
	}


public:

	SumSubscriber(): Node("sum_subscriber"), value_a(0), value_b(0), has_a(false), has_b(false), compute_value(0){
	m_publisher_sum = this->create_publisher<std_msgs::msg::Int32>("topic_sum",10);
	m_subscriber_a = this->create_subscription<std_msgs::msg::Int32>("topic_a",10,std::bind(&SumSubscriber::callback_a, this, std::placeholders::_1));	
        m_subscriber_b = this->create_subscription<std_msgs::msg::Int32>("topic_b",10,std::bind(&SumSubscriber::callback_b, this, std::placeholders::_1));     
	}

};

int main(int argc, char **argv){
	rclcpp::init(argc,argv);
	rclcpp::spin(std::make_shared<SumSubscriber>());
	rclcpp::shutdown();

return 0;

}

