#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <std_msgs/Float32MultiArray.h>

///options///
//#define demo
#define video_480p
//#define video_720p
//#define video_30fps
#define video_60fps
////////////

///set camera offset///
#ifdef video_480p
int x_offset = 320;
int y_offset = 240;
#endif

#ifdef video_720p
int x_offset = 540;
int y_offset = 360;
#endif
////////////

///collor range///
struct orange{
  int hue_min_ = 55;
  int hue_max_ = 95;
  int sat_min_ = 125;
  int sat_max_ = 255;
  int val_min_ = 25;
  int val_max_ = 135;
};
struct orange orange;
////////////

///Mat frame///
cv::Mat frame ,hsv , frame_orange , frame_orange_glay;
////////////

///set moment point///
cv::Moments mu_orange;
cv::Point2f mc_orange;
////////////

///middle point x y allay///
std_msgs::Float32MultiArray ball_point_array;
////////////

using namespace cv;

int main(int argc, char** argv){
    ros::init(argc, argv, "cam_info_publisher");
	ros::NodeHandle nh;
	
	ros::Publisher pub = nh.advertise<std_msgs::Float32MultiArray>("ball_point_array", 1);
	
	ros::Rate rate(60);

    VideoCapture capture(-1); // capture video
    
#ifdef video_30fps
    capture.set(CAP_PROP_FPS, 30);  //set video fps
#endif
#ifdef video_60fps
    capture.set(CAP_PROP_FPS, 60);  //set video fps
#endif

#ifdef video_480p
    capture.set(CAP_PROP_FRAME_WIDTH, 640);  //set video frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);  //set video frame height
#endif
#ifdef video_720p
    capture.set(CAP_PROP_FRAME_WIDTH, 1080);  //set video frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, 720);  //set video frame height
#endif

#ifdef demo
    char Example[] = "cam collor";
    namedWindow(Example, WINDOW_AUTOSIZE);  //create window
#endif

    while(ros::ok()){
        capture >> frame;

        frame_orange = frame.clone();  //clone frame to frame_orange
        
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        
        for(int y = 0; y < hsv.rows; y++){
            for(int x = 0; x < hsv.cols; x++){
                Vec3b data = hsv.at<Vec3b>(y,x);
                if(data[0] < orange.hue_min_ || data[0] > orange.hue_max_ || data[1] < orange.sat_min_ || data[1] > orange.sat_max_ || data[2] < orange.val_min_ || data[2] > orange.val_max_){
                    frame_orange.at<Vec3b>(y,x) = Vec3b(0, 0, 0);
                }
                else{
                    frame_orange.at<Vec3b>(y,x) = Vec3b(255, 255, 255);
                }
            }
        }

        cvtColor(frame_orange, frame_orange_glay, COLOR_BGR2GRAY); //convert frame_orange to glay scale

        mu_orange = moments(frame_orange_glay, true);
        mc_orange = Point2f( mu_orange.m10/mu_orange.m00 , mu_orange.m01/mu_orange.m00 );
#ifdef demo
        circle(frame_orange, mc_orange, 4, Scalar(100), 2, 4);
#endif

        ball_point_array.data.resize(2);
        ball_point_array.data[0] = mc_orange.x - x_offset;
        ball_point_array.data[1] = y_offset - mc_orange.y;
        
        pub.publish(ball_point_array);

        printf("x: %f  y: %f\n", ball_point_array.data[0], ball_point_array.data[1]);

#ifdef demo
        imshow(Example, frame_orange);  //show frame_orange
#endif
        
        waitKey(1);
    }
    return(0);    
}