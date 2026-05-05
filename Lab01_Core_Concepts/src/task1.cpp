#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "sensor_msgs/msg/temperature.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <fstream>
#include <string>

using namespace std::chrono_literals;

class  CpuTempPublisher : public rclcpp::Node{
private:
rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr m_cpu_temp_publisher;
rclcpp::TimerBase::SharedPtr m_timer;

size_t value;

std::string filename{"/sys/class/thermal/thermal_zone0/temp"};

void timer_callback(){
	auto msg = sensor_msgs::msg::Temperature() ;
	// Read from the text file
	std::ifstream MyReadFile(filename);
	std::string reading;
	std::getline(MyReadFile, reading);
	msg.temperature = std::stod(reading)/1000;
	
	msg.header.stamp = this->get_clock()->now();

	m_cpu_temp_publisher->publish(msg);
	RCLCPP_INFO(this->get_logger(),"CPU Temperature Publisher data : %.1f",msg.temperature);
	}

public:
	CpuTempPublisher(): Node("cpu_temp"){
		m_cpu_temp_publisher  = this->create_publisher<sensor_msgs::msg::Temperature>("cpu_temp",10);
		m_timer = this->create_wall_timer(1s,std::bind(&CpuTempPublisher::timer_callback,this));
	}


};

int main(int argc, char ** argv)
{
  rclcpp::init(argc,argv);
  rclcpp::spin(std::make_shared<CpuTempPublisher>());
  rclcpp::shutdown();
  return 0;
}
