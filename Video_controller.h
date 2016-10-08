//
// Created by nikita on 08.10.16.
//

#ifndef CALIBRATION_VEDEO_CONTROLLER_H
#define CALIBRATION_VEDEO_CONTROLLER_H
#include "My_exeptions.h"


class Video_controller {
private:
    int camera_id;
    VideoCapture cap;
    //
    int iLowH = 0;
    int iHighH = 255;
    int iLowS = 0;
    int iHighS = 255;
    int iLowV = 0;
    int iHighV = 255;
    //
    int CATCH = 0;
    int MAP = 0;
    //
    Mat img_original;
    Mat rHSV;
    Mat q;
    Mat sum;

    void create_windows();

    void create_track_bar();

    void create_track_bar_bool();
public:

    Video_controller(int camera_id);

    void start();

    Mat& get_sum();

    bool is_reafing();

    bool is_opened();

    bool get_map();


};


#endif //CALIBRATION_VEDEO_CONTROLLER_H
