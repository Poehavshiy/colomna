//
// Created by nikita on 07.10.16.
//

#ifndef CALIBRATION_CAMERA_CALIBRATOR_H
#define CALIBRATION_CAMERA_CALIBRATOR_H
#include "Smart_point2f.h"


class Camera_calibrator {
private:
    ofstream log_file;

    string path_to_files = "/home/nikita/colomna/calibration/text/" ;

    string log_file_name = "calibration_log.txt";

    string result_file_name;

    //number of points needed for calibrtion
    int calib_points_n;

    //points from calibration picture and from frame, indeed
    vector<Smart_point2f> from_picture;
    vector<Smart_point2f> from_frame;

    //result_calibration matrix
    Mat calibration_result;

    //path to the calibration picture and for result output
    string calib_picture_path;
    string path_to_result_output;

    //id of usb camera
    int camera_id;

    //pictures from calibration picture and from frame, indeed
    Mat calibration_picture;
    Mat target_frame;
    //
    void fill_point_vector( vector<Smart_point2f>& target, Mat& target_pic );


public:

    Camera_calibrator(int calib_points_n, int camera_id,
                      string calib_picture_path, string path_to_result_output);

    void fill_from_picture();

    void fill_from_frame();

    void set_calibration_picture( Mat& target_frame );

    void calibrate();

    void write_to_file();

    ofstream& get_log_file();

    ~Camera_calibrator();




};


#endif //CALIBRATION_CAMERA_CALIBRATOR_H
