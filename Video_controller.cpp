//
// Created by nikita on 08.10.16.
//

#include "Video_controller.h"

void Video_controller::create_windows(){
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    moveWindow("Original", 0, 0);

    namedWindow("After ColourFilt", CV_WINDOW_AUTOSIZE);
    moveWindow("After ColourFilt", 700, 0);

    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 350, 350);
}

void Video_controller::create_track_bar(){
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 255);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

void Video_controller::create_track_bar_bool(){
    namedWindow("CATCH@MAP", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 500, 350);
    cvCreateTrackbar("CATCH", "CATCH@MAP", &CATCH, 1); //Hue (0 - 179)
    cvCreateTrackbar("MAP", "CATCH@MAP", &MAP, 1);
}

Video_controller::Video_controller(int camera_id):cap(camera_id){
    create_windows();
    create_track_bar();
    create_track_bar_bool();
    this->camera_id = camera_id;
}

void Video_controller::start(){
    bool bSuccess = cap.read(img_original);
    //
    cvtColor(img_original, rHSV, COLOR_BGR2HSV);
    inRange(rHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), q);
    cvtColor(q, q, CV_GRAY2BGR);
    cvtColor(rHSV, rHSV, COLOR_HSV2BGR);
    bitwise_and(rHSV, q, sum);
    imshow("After ColourFilt", sum);
    imshow("Original", img_original);
}

Mat& Video_controller::get_sum(){
    return sum;
}

bool Video_controller::get_map(){
    return MAP;
}


bool Video_controller::is_opened(){
    return cap.isOpened();
}
