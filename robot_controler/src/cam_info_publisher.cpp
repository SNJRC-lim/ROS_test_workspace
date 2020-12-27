#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <std_msgs/Float32MultiArray.h>
#include <chrono>

///options///
//#define demo
#define timer
#define video_480p
//#define video_720p
//#define video_30fps
//#define video_60fps
#define video_120fps
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
cv::Mat frame ,hsv , frame_orange;
////////////

///set moment point///
cv::Moments mu_orange;
cv::Point2f mc_orange;
////////////

///middle point x y allay///
std_msgs::Float32MultiArray ball_point_array;
////////////

using namespace cv;
using namespace std::chrono;
inline double get_time_sec(void){
    return static_cast<double>(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count())/1000000000;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "cam_info_publisher");
	ros::NodeHandle nh;
	
	ros::Publisher pub = nh.advertise<std_msgs::Float32MultiArray>("ball_point_array", 1);
	
	ros::Rate rate(120);

    VideoCapture capture(-1); // capture video
    
#ifdef video_30fps
    capture.set(CAP_PROP_FPS, 30);  //set video fps
#endif
#ifdef video_60fps
    capture.set(CAP_PROP_FPS, 60);  //set video fps
#endif
#ifdef video_120fps
    capture.set(CAP_PROP_FPS, 120);  //set video fps
#endif

#ifdef video_480p
    capture.set(CAP_PROP_FRAME_WIDTH, 640);  //set video frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);  //set video frame height
#endif
#ifdef video_720p
    capture.set(CAP_PROP_FRAME_WIDTH, 1080);  //set video frame width
    capture.set(CAP_PROP_FRAME_HEIGHT, 720);  //set video frame height
#endif

    capture.set(CAP_PROP_BUFFERSIZE, 4);
    capture.set(CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G'));
    capture.set(CAP_PROP_EXPOSURE, 115);

#ifdef demo
    char Example[] = "cam collor";
    namedWindow(Example, WINDOW_AUTOSIZE);  //create window
#endif

    while(ros::ok()){
#ifdef timer
        double start,end;
        start = get_time_sec();
#endif

        capture >> frame;

        cvtColor(frame, hsv, COLOR_BGR2HSV);

        frame.release();

        frame_orange = Mat(hsv.size(), CV_8UC1, Scalar(0));

        int hsv_rows = hsv.rows;
        int hsv_cols = hsv.cols;

#pragma omp parallel for    
        for(int y = 0; y < hsv_rows; y++){
            const uchar *ptr_hsv = hsv.ptr<uchar>(y);
            uchar *ptr_frame_orange = frame_orange.ptr<uchar>(y);
#pragma omp parallel for
            for(int x = 0; x < hsv_cols; x++){
                uchar hue = ptr_hsv[3 * x + 0];
                uchar sat = ptr_hsv[3 * x + 1];
                uchar val = ptr_hsv[3 * x + 2];
                if(hue < orange.hue_min_ || hue > orange.hue_max_)continue;
                if(sat < orange.sat_min_ || sat > orange.sat_max_)continue;
                if(val < orange.val_min_ || val > orange.val_max_)continue;
                ptr_frame_orange[x] = 255;
            }
        }

        hsv.release();

        mu_orange = moments(frame_orange, true);
        mc_orange = Point2f( mu_orange.m10/mu_orange.m00 , mu_orange.m01/mu_orange.m00 );

#ifdef demo
        circle(frame_orange, mc_orange, 4, Scalar(100), 2, 4);
#endif

        ball_point_array.data.resize(2);
        ball_point_array.data[0] = mc_orange.x - x_offset;
        ball_point_array.data[1] = y_offset - mc_orange.y;
        
        pub.publish(ball_point_array);

        //printf("x: %f  y: %f\n", ball_point_array.data[0], ball_point_array.data[1]);
        //ROS_INFO("published");

#ifdef demo
        imshow(Example, frame_orange);  //show frame_orange
#endif

        frame_orange.release();
        
#ifdef timer       
        end = get_time_sec();
        double elapsed=end-start;
        printf("time: %f\n", elapsed);
#endif
    }
    return(0);    
}