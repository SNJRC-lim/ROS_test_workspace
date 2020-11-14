#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/ByteMultiArray.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16MultiArray.h>
#include <mutex>
#include <math.h>

///options///
//#define demo

////////////

///Publish Data///
static std_msgs::Int16MultiArray robot_go_array; //[0] == angle, [1] == speed
static std_msgs::Bool ball_kick;

///ball middle point x y allay///
static std_msgs::Float32MultiArray ball_point_array;
////////////

///from serial node///
static std_msgs::ByteMultiArray arduino_info_array;
////////////

///set mutex///
static std::mutex mutex;
////////////

///Call Back funcsions///
void CamInfoCallback(const std_msgs::Float32MultiArray &ball){
  std::lock_guard<std::mutex> lock(mutex);

  for (int i = 0; i < 2; i++){
    ball_point_array.data[i] = ball.data[i];
  }
  
#ifdef demo
  int num = ball.data.size();
  ROS_INFO("I susclibed [%i]", num);
  for (int i = 0; i < num; i++){
    ROS_INFO("[%i]:%f", i, ball.data[i]);
  }
#endif 
}

void ArduinoInfoCallback(const std_msgs::ByteMultiArray &arduino){
  std::lock_guard<std::mutex> lock(mutex);

  for (int i = 0; i < 2; i++){
    arduino_info_array.data[i] = arduino.data[i];
  }
}
/////////////

///Coordinate conversion funcsions///
float ball_angle(){
  return atan2f(ball_point_array.data[1], ball_point_array.data[0]);
}
/////////////
int main(int argc, char** argv){
  ball_point_array.data.resize(2);
  robot_go_array.data.resize(2);

  ros::init(argc, argv, "main_controler");
  ros::NodeHandle nh;
  
  ros::Subscriber sub1 = nh.subscribe("ball_point_array", 1, CamInfoCallback);
  ros::Subscriber sub2 = nh.subscribe("arduino_info_array", 1, ArduinoInfoCallback);

  ros::Publisher pub1 = nh.advertise<std_msgs::Int16MultiArray>("robot_go_array", 1);
  ros::Publisher pub3 = nh.advertise<std_msgs::Bool>("ball_kick", 1);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  while(ros::ok()){
    mutex.lock();
    
    printf("angle: %f\n", ball_angle());

    mutex.unlock();
  }
  return 0;
}