//
// Created by nikita on 07.10.16.
//

#include "Camera_calibrator.h"
Camera_calibrator::~Camera_calibrator(){
    log_file.close();
}

Camera_calibrator::Camera_calibrator(int calib_points_n, int camera_id,
                                     string calib_picture_path, string  result_file_name) {
    log_file.open(path_to_files + log_file_name);
    //
    this->calib_points_n = calib_points_n;
    this->camera_id = camera_id;
    this->calib_picture_path = calib_picture_path;
    this->result_file_name = result_file_name + to_string(camera_id) + ".txt";
    //
    calibration_picture = imread(calib_picture_path);
}

//
void Camera_calibrator::fill_from_picture() {
    try {
        fill_point_vector(from_picture, calibration_picture);
        sort(from_picture.begin(),  from_picture.end());
        log_file << " Getting points from calibration pic was successful "<<endl;

    }
    catch (My_exeption &ex) {
        cout << ex.what() << '\n';
        ex.write_to_log_additional("Exeption occured in fill_from_picture() while "
                                           "filling from_picture point vector\n", log_file);
    }
}

//
void Camera_calibrator::fill_from_frame() {
    try {
        fill_point_vector(from_frame, target_frame);
        sort(from_frame.begin(),  from_frame.end());
        log_file << " Getting points from frame was successful "<<endl;

    }
    catch (My_exeption &ex) {
        cout << ex.what() << '\n';
        ex.write_to_log_additional("Exeption occured in fill_from_frame() while "
                                           "filling from_frame point vector\n", log_file);

    }
}

//
void Camera_calibrator::fill_point_vector(vector<Smart_point2f> &target, Mat &target_pic) {

    cvtColor(target_pic, target_pic, CV_BGR2GRAY);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Moments mu;
    findContours(target_pic, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    unsigned long countursN = contours.size();
    string path_to_log_file = path_to_files + log_file_name;
    if (countursN == 0) {
        cout << "Countors have not been found" << endl;
        No_countors no_points(path_to_log_file);
        no_points.write_to_log_additional("Contours = " + std::to_string(0) + "\n", log_file);
        throw no_points;
    }
    if (countursN < calib_points_n) {
        cout << "Too few contours" << endl;
        Few_countors few_countors(path_to_log_file);
        few_countors.write_to_log_additional("Contours = " + std::to_string(countursN) + "\n", log_file);
        throw few_countors;
    }
    if (countursN > calib_points_n) {
        cout << "Too many contours" << endl;
        Many_countors many_countors(path_to_log_file);
        many_countors.write_to_log_additional("Contours = " + std::to_string(countursN) + "\n", log_file);
        throw many_countors;
    }

    for (int i = 0; i < countursN; i++) {
        mu = moments(contours[i], false);
        Smart_point2f current(mu.m10 / mu.m00, mu.m01 / mu.m00);
        target.push_back(current);
    }
 }

//
void Camera_calibrator::set_calibration_picture( Mat& target_frame ){
    this->target_frame = target_frame.clone();
}
//
void Camera_calibrator::calibrate(){
    if(from_picture.size() != 0 && ( from_picture.size() == from_frame.size())) {
        vector<Point2f> inputQuad;
        vector<Point2f> outputQuad;
        for (auto i : from_picture) {
            inputQuad.push_back(i.getData());
        }

        for (auto i : from_frame) {
            outputQuad.push_back(i.getData());
        }
        calibration_result = getPerspectiveTransform(inputQuad, outputQuad);
    }
    else {
        string path_to_log_file = path_to_files + log_file_name;
        cout << "Not enough data" << endl;
        Not_enough_data not_enough_data(path_to_log_file);
        not_enough_data.write_to_log_additional("Size of from_picture = " +
                                                        std::to_string(from_picture.size()) + "\n", log_file);
        not_enough_data.write_to_log_additional("Size of from_frame = " +
                                                std::to_string(from_frame.size()) + "\n", log_file);

        throw not_enough_data;
    }

}

void Camera_calibrator:: write_to_file(){
    if(calibration_result.empty()){
        string path_to_log_file = path_to_files + log_file_name;
        Empty_result empty_result(path_to_log_file);
        empty_result.write_to_log_additional("Mat was empty, then exeption occured", log_file);
        cout << "Result mat is empty" << endl;
        throw empty_result;
    }
    std::ofstream outfile (path_to_files + log_file_name);
    int i, j;
    for (i = 0; i < calibration_result.rows; i++) {
        for (j = 0; j < calibration_result.cols; j++) {
            outfile<<calibration_result.at<double>(i, j)<<' ';
        }
        cout<<endl;
    }
    outfile.close();
}
//
ofstream& Camera_calibrator::get_log_file(){
    return log_file;
}