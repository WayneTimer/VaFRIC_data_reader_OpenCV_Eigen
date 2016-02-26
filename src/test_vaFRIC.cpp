#include <iostream>
#include <stdio.h>
#include "VaFRIC/VaFRIC.h"

#include <cvd/videodisplay.h>
#include <cvd/gl_helpers.h>

using namespace std;
using namespace CVD;

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

    CVD::Image< CVD::Rgb<CVD::byte> > in;
    in = CVD::img_load("./test_data/test.png");

    CVD::VideoDisplay window(in.size());
    glDrawPixels(in);

    glColor3f(1,0,0);
    glBegin(GL_LINE);
    glVertex(in.size()/2);
    glVertex(in.size());
    glEnd();
    glFlush();

    cout<<"done!"<<endl;
    cin.get();
    return 0;
}
