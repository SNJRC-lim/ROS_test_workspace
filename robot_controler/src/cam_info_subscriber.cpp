#include <std_msgs/Float32MultiArray.h>
#include <ros/ros.h>

void chatterCallback(const std_msgs::Float32MultiArray &msg)
{
  int num = msg.data.size();
  ROS_INFO("I susclibed [%i]", num);
  for (int i = 0; i < num; i++)
  {
    ROS_INFO("[%i]:%f", i, msg.data[i]);
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "cam_info_subscriber");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("ball_point_array", 1, chatterCallback);

  ros::spin();
  return 0;
}
