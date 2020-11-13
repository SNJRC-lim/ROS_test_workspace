#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int8.h>
#include <mutex>

///options///
//#define demo

////////////

///Publish Data///
static std_msgs::Int16 robot_go_angle; //-314~314
static std_msgs::Int8 robot_go_speed; //0~255

///ball middle point x y allay///
static std_msgs::Float32MultiArray ball_point_array;
////////////

///from serial node///
static std_msgs::Bool robot_still_alive;
static std_msgs::Bool processing_white_line;
////////////

///set mutex///
static std::mutex mutex;
////////////

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

void ArduinoInfo1Callback(const std_msgs::Bool &alive){
  std::lock_guard<std::mutex> lock(mutex);

  robot_still_alive.data = alive.data;  
}

void ArduinoInfo2Callback(const std_msgs::Bool &white){
  std::lock_guard<std::mutex> lock(mutex);

  processing_white_line.data = white.data; 
}

int main(int argc, char** argv){
  ball_point_array.data.resize(2);
  ros::init(argc, argv, "main_controler");
  ros::NodeHandle nh;
  
  ros::Subscriber sub1 = nh.subscribe("ball_point_array", 1, CamInfoCallback);
  ros::Subscriber sub2 = nh.subscribe("robot_still_alive", 1, ArduinoInfo1Callback);
  ros::Subscriber sub3 = nh.subscribe("processing_white_line", 1, ArduinoInfo2Callback);

  ros::Publisher pub1 = nh.advertise<std_msgs::Int16>("robot_go_angle" , 1);
  ros::Publisher pub2 = nh.advertise<std_msgs::Int8>("robot_go_speed" , 1);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  while(ros::ok()){
    mutex.lock();
    
    printf("x: %f  y: %f\n", ball_point_array.data[0], ball_point_array.data[1]);

    mutex.unlock();
  }
  return 0;
}