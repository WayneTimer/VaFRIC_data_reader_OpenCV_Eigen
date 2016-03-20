#include <cstdio>

#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

#include <eigen3/Eigen/Dense>

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include "VaFRIC/VaFRIC.h"

using namespace std;

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

#define HZ 100
#define Y_LIMIT 10

char dir[] = "/home/timer/VaFRIC_datasets/test/100Hz";

int main(int argc, char **argv)
{
    ros::init(argc,argv,"publisher");
    ros::NodeHandle node;
    
    ros::Publisher img_pub = node.advertise<sensor_msgs::Image>("image_raw",1000);
    ros::Publisher depth_img_pub = node.advertise<sensor_msgs::Image>("depth_img",1000);
    ros::Publisher cam_pose_pub = node.advertise<nav_msgs::Odometry>("cam_pose",1000);
    ros::Publisher imu_pub = node.advertise<sensor_msgs::Imu>("imu",1000);

    // init VaFRIC
    float K[3][3] = { 481.20,   0,  319.50,
                        0,  -480.00,    239.50,
                        0,  0,  1.00};

    dataset::vaFRIC dataset(dir,IMG_WIDTH,IMG_HEIGHT,
                            K[0][2],K[1][2],
                            K[0][0],K[1][1]);

    ros::Time time_stamp = ros::Time::now();
    ros::Duration time_frequency(1.0/HZ);
    int x,y;
    x = 0;
    y = 0;
    
    sleep(2);

    while (ros::ok() && y<Y_LIMIT)
    {
        char file_path[100];
        
        // publish img_raw
        cv_bridge::CvImage img;
        img.header.stamp = time_stamp;
        img.image = dataset.getPNGImage(y,x);
        img.encoding = "bgr8";
        img_pub.publish(img.toImageMsg());

        // publish depth
        cv::Mat depth_img(480,640,CV_32FC1);
        FILE *depth_file;
        sprintf(file_path,"%s/scene_%02d_%04d.depth",dir,x,y);
        puts(file_path);
        printf("%lf\n",time_stamp.toSec());
        depth_file = fopen(file_path,"r");
        float depth_data[640*480];
        for (int i=0;i<640*480;i++)
        {
            fscanf(depth_file,"%f",&depth_data[i]);
        }
        fclose(depth_file);
        memcpy(depth_img.data,depth_data,sizeof(depth_data));
        img.image = depth_img.clone();
        img.encoding = "32SC1";
        depth_img_pub.publish(img.toImageMsg());

        // publish cam_pose
        Eigen::MatrixXf RT;
        RT = dataset.computeTpov_cam(y,x);
        Eigen::Matrix3f R;
        Eigen::Vector3f T;
        R = RT.topLeftCorner<3,3>();
        T = RT.col(3);
        nav_msgs::Odometry odo;
        odo.header.stamp = time_stamp;
        odo.pose.pose.position.x = T(0);
        odo.pose.pose.position.y = T(1);
        odo.pose.pose.position.z = T(2);
        Eigen::Quaternionf q = Eigen::Quaternionf(R);
        odo.pose.pose.orientation.w = q.w();
        odo.pose.pose.orientation.x = q.x();
        odo.pose.pose.orientation.y = q.y();
        odo.pose.pose.orientation.z = q.z();
        cam_pose_pub.publish(odo);

        sleep(1);

        y++;
        time_stamp = time_stamp + time_frequency;
    }
    return 0;
}
