#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <mutex>

///options///
//#define demo

////////////

///ball middle point x y allay///
static std_msgs::Float32MultiArray ball_point_array;
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

int main(int argc, char** argv){
  ball_point_array.data.resize(2);
  ros::init(argc, argv, "main_controler");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("ball_point_array", 1, CamInfoCallback);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  while(ros::ok()){
    mutex.lock();
    
    printf("x: %f  y: %f\n", ball_point_array.data[0], ball_point_array.data[1]);

    mutex.unlock();
  }
  return 0;
}