#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/ByteMultiArray.h>
#include <std_msgs/Bool.h>
#include <mutex>
#include <math.h>
#include <unistd.h>

///options///
//#define demo

////////////

///Publish Data///
static std_msgs::Float32MultiArray robot_go_array; //[0] == angle, [1] == speed
static std_msgs::Bool ball_kick;

///ball middle point x y allay///
static std_msgs::Float32MultiArray ball_point_array;
////////////

///from serial node///
static std_msgs::ByteMultiArray arduino_info_array;
////////////

///ball angle///
static float ball_angle;
////////////

///set mutex///
static std::mutex mutex;
////////////

static long i = 0;

///Call Back funcsions///
void CamInfoCallback(const std_msgs::Float32MultiArray &ball){
  std::lock_guard<std::mutex> lock(mutex);

  for (int i = 0; i < 2; i++){
    ball_point_array.data[i] = ball.data[i];
  } 
}

void ArduinoInfoCallback(const std_msgs::ByteMultiArray &arduino){
  std::lock_guard<std::mutex> lock(mutex);

  for (int i = 0; i < 3; i++){
    arduino_info_array.data[i] = arduino.data[i];
  }
}
/////////////

///Coordinate conversion funcsions///
float ball_angle_conv(){
  return atan2f(ball_point_array.data[1], ball_point_array.data[0]);
}
/////////////

///decide where to go///
void robot_go_angle() {
  /*if (ball_angle <= 0) {
    ball_angle = ball_angle + M_PI;
  }
  else if (0 < ball_angle) {
    ball_angle = ball_angle - M_PI;
  }

  if ((M_PI / 3 <= ball_angle) && (ball_angle <= 2 * M_PI / 3)) {
    robot_go_array.data[0] = ball_angle;
  }


  if (((0 <= ball_angle) && (ball_angle < M_PI / 12)) || ((11 * M_PI / 12 < ball_angle) && (ball_angle <= M_PI))) {
    robot_go_array.data[0] = -M_PI / 2;
  }

  else if ((M_PI / 12 <= ball_angle) && (ball_angle < M_PI / 3)) {
    robot_go_array.data[0] = ball_angle - M_PI / 2;
  }

  else if ((2 * M_PI / 3 < ball_angle) && (ball_angle <= 11 * M_PI / 12)) {
    robot_go_array.data[0] = ball_angle - 3 * M_PI / 2;
  }

  else if ((-M_PI / 2 <= ball_angle) && (ball_angle < 0)) {
    robot_go_array.data[0] = -M_PI / 2 + ball_angle;
  }
  else if ((-M_PI <= ball_angle) && (ball_angle < -M_PI / 2 )) {
    robot_go_array.data[0] = M_PI / 2 + ball_angle;
  }*/
}
////////////////


int main(int argc, char** argv){
  ball_point_array.data.resize(2);
  robot_go_array.data.resize(3);

  ros::init(argc, argv, "main_controler");
  ros::NodeHandle nh;
  
  ros::Subscriber sub1 = nh.subscribe("ball_point_array", 1, CamInfoCallback);
  ros::Subscriber sub2 = nh.subscribe("arduino_info_array", 1, ArduinoInfoCallback);

  ros::Publisher pub1 = nh.advertise<std_msgs::Float32MultiArray>("robot_go_array", 1);
  ros::Publisher pub2 = nh.advertise<std_msgs::Bool>("ball_kick", 1);

  ros::AsyncSpinner spinner(1);
  spinner.start();

  while(ros::ok()){
    mutex.lock();
    
    //ball_angle = ball_angle_conv();
    //robot_go_angle();
    //i = i + 1;

    /*if(i <= 1000){
      robot_go_array.data[0] = 0;
    }

    else if(i <= 2000){
      robot_go_array.data[0] = M_PI/6;
    }

    else if(i <= 3000){
      robot_go_array.data[0] = M_PI/4;
    }

    else if(i <= 4000){
      robot_go_array.data[0] = M_PI/3;
    }

    else if(i <= 5000){
      robot_go_array.data[0] = M_PI/2;
    }
    
    else if(i <= 6000){
      robot_go_array.data[0] = 2*M_PI/3;
    }
  
    else if(i <= 7000){
      robot_go_array.data[0] = 3*M_PI/4;
    }

    else if(i <= 8000){
      robot_go_array.data[0] = 5*M_PI/6;
    }

    else if(i <= 9000){
      robot_go_array.data[0] = M_PI;
    }

    else if(i <= 10000){
      robot_go_array.data[0] = -M_PI;
    }

    else if(i <= 11000){
      robot_go_array.data[0] = -M_PI/6;
    }

    else if(i <= 12000){
      robot_go_array.data[0] = -M_PI/4;
    }

    else if(i <= 13000){
      robot_go_array.data[0] = -M_PI/3;
    }

    else if(i <= 14000){
      robot_go_array.data[0] = -M_PI/2;
    }

    else if(i <= 15000){
      robot_go_array.data[0] = -2*M_PI/3;
    }

    else if(i <= 16000){
      robot_go_array.data[0] = -3*M_PI/4;
    }
    
    else if(i <= 17000){
      robot_go_array.data[0] = -5*M_PI/6;

      i = 0;
    }*/
    
    robot_go_array.data[0] = M_PI/3;
    robot_go_array.data[1] = 100;

    pub1.publish(robot_go_array);
    ball_kick.data = 0;
    pub2.publish(ball_kick);
    printf("angle: %f  speed: %f\n", robot_go_array.data[0], robot_go_array.data[1]);
    //printf("i: %f\n", i);

    mutex.unlock();
    usleep(15000);
  }
  return 0;
}