#include <iostream>
#include <stdio.h>
#include "VaFRIC/VaFRIC.h"

#include <opencv2/opencv.hpp>

using namespace std;

void show_mat(cv::Mat &img,string name)
{
    cout << name << " property:" << endl;
    cout << "dims: " << img.dims << endl;
    cout << "rows: " << img.rows << endl;
    cout << "cols: " << img.cols << endl;
    cout << "channels: " << img.channels() << endl;
    cout << "type: " << img.type() << endl;
    cout << "depth: " << img.depth() << endl;
    cout << "elemSize: " << img.elemSize() << endl;
    cout << "elemSize1: " << img.elemSize1() << endl;
}

int main(void)
{

    float K[3][3] = { 481.20,   0,  319.50,
             0, -480.00,  239.50,
             0,         0,    1.00 };

    //dataset::vaFRIC dataset("/home/ankur/workspace/GT/SlowTraj/20fps/",
    dataset::vaFRIC dataset("/home/timer/temp/20fps_real_GT_archieve/",
                     640,480,
                     K[0][2],
                     K[1][2],
                     K[0][0],
                     K[1][1]);

    cout<<"Number of text files = " << dataset.getNumberofPoseFiles()<< endl;

    cv::Mat img;
    img = cv::imread("./test_data/test.png");
    show_mat(img,"img");
    cv::imshow("foo",img);
    cv::waitKey(0);
    return 0;
}
