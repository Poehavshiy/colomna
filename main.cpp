#include "Camera_calibrator.h"
#include "Video_controller.h"
const int NforCalibr = 4;

int main() {
    Video_controller video_controller(0);
    VideoCapture cap(0);

    Camera_calibrator camera_calibrator(NforCalibr, 1,
                                        "/home/nikita/colomna/calibration/calibration.png",
                                        "transform");
    camera_calibrator.fill_from_picture();
    if (!video_controller.is_opened()) {  // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    while (true) {
        video_controller.start();

        if (waitKey(30) == 'q') {
            Mat Calib1;
            Calib1 = video_controller.get_sum();

            camera_calibrator.set_calibration_picture(Calib1);
            camera_calibrator.fill_from_frame();
        }
        if (video_controller.get_map() != 0) {
            try {
                camera_calibrator.calibrate();
            }
            catch (My_exeption &ex) {
                cout << ex.what() << '\n';
            }
        }


        if (waitKey(30) == 27) {
            try {
                camera_calibrator.write_to_file();
            }
            catch (My_exeption &ex) {
                cout << ex.what() << '\n';
            }
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    return (0);
}
