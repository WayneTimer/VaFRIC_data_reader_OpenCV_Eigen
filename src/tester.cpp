#include <cstdio>

#include <vector>

#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

#include <eigen3/Eigen/Dense>

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

#define HZ 100
#define Y_LIMIT 10

char dir[] = "/home/timer/catkin_ws/output/";

int png_y = 0;
int depth_y = 0;

vector<cv::Mat> depth_vec;

void save_depth(cv::Mat img)
{
    float depth_data[640*480];
    memcpy(depth_data,img.data,sizeof(depth_data));
    char file_path[100];
    sprintf(file_path,"%sscene_00_%04d.depth",dir,depth_y++);
    FILE *depth_file;
    depth_file = fopen(file_path,"w");
    fprintf(depth_file,"%.3f",depth_data[0]);
    for (int i=1;i<640*480;i++)
    {
        fprintf(depth_file," %.3f",depth_data[i]);
    }
    fclose(depth_file);
}

void image_callback(const sensor_msgs::Image& msg)
{
    cv::Mat img;
    img = cv_bridge::toCvCopy(msg,string("8UC3"))->image;
    char file_path[100];
    sprintf(file_path,"%sscene_00_%04d.png",dir,png_y++);
    cv::imwrite(file_path,img);
}

void depth_callback(const sensor_msgs::Image& msg)
{
    cv::Mat img;
    img = cv_bridge::toCvCopy(msg,string("32SC1"))->image;
    depth_vec.push_back(img);
    printf("%d : %lf\n",depth_y,msg.header.stamp.toSec());
    save_depth(img);
}

void cam_pose_callback(const nav_msgs::Odometry& msg)
{
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"tester");
    ros::NodeHandle node;

    depth_vec.clear();
    
    ros::Subscriber img_pub = node.subscribe("image_raw",1000,image_callback);
    ros::Subscriber depth_img_rec = node.subscribe("depth_img",1000,depth_callback);
    ros::Subscriber cam_pose_pub = node.subscribe("cam_pose",1000,cam_pose_callback);
    //ros::Subscriber imu_pub = node.subscribe("imu",1000,imu_callback);

    ros::spin();
    /*
        // publish 
        cv::Mat depth_img(480,640,CV_32FC1);
        FILE *depth_file;
        sprintf(file_path,"%sscene_%02d_%04d.depth",dir,x,y);
        depth_file = fopen(file_path,"r");
        float depth_data[640*480];
        for (int i=0;i<640*480;i++)
        {
            fscanf(depth_file,"%f",&depth_data[i]);
        }
        memcpy(depth_img.data,depth_data,sizeof(depth_data));
        img.image = depth_img.clone();
        img.encoding = "32SC1";
        depth_img_pub.publish(img.toImageMsg());

        sleep(1);

        y++;
        time_stamp = time_stamp + time_frequency;
    */
    return 0;
}
