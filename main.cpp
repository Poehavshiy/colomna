#include "MYfunctions.h"

const int NforCalibr = 4;

int main() {
    FILE *FileM;
    ///home/nikita/sm6_colomna/project/calibration
    FileM = fopen("/home/nikita/sm6_colomna/project/calibration/Mcolour.txt", "r+");
    fprintf(FileM, "Some text: %s\n", "sfbsedfb");

    vector<Point2f> obj;
    vector<Point2f> scene;
    Point2f *inputQuad = 0;
    Point2f *outputQuad = 0;
    Mat Calib = imread("/home/nikita/sm6_colomna/project/calibration/calibration.png");
    bool CalStat = 0, CheckStat = 0;
    Mat *calib = &Calib;
    inputQuad = contorsForCalib(Calib, NforCalibr);


    DirtySort(inputQuad, NforCalibr);

    check(Calib, inputQuad, NforCalibr);
    imshow("Real", Calib);
    imwrite("CheckCatchOrin.png", Calib);
    cout << "inputQuad:" << endl;
    for (int i = 0; i < NforCalibr; ++i) {
        cout << inputQuad[i].x << ' ' << inputQuad[i].y << endl;
    }

    Create_windows();
    TrackBar(iLowH, iHighH, iLowS, iHighS, iLowV, iHighV);
    TrackBarBool(CATCH, MAP);
    Mat imgOriginal;
    Mat imgHist;
    Mat imgThresholded;
    Mat RHSV, SUM;//
    Mat Q;
    Mat lambda, lambdainv;
    Mat *LAMBDA = &lambda;
    Mat *LAMBDAinv = &lambdainv;

    Mat *Orig = &imgOriginal, *Hist = &imgHist, *Thresholded = &imgThresholded;
    Mat *q = &Q, *rhsv = &RHSV, *sum = &SUM;
    Vec3b intensity;
    VideoCapture cap(0);
    if (!cap.isOpened()) {  // if not success, exit program
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    while (true) {
        bool bSuccess = cap.read(imgOriginal);
        if (!bSuccess) { //if not success, break loop
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        cvtColor(imgOriginal, RHSV, COLOR_BGR2HSV);
        inRange(RHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), Q);
        cvtColor(Q, Q, CV_GRAY2BGR);
        cvtColor(RHSV, RHSV, COLOR_HSV2BGR);
        bitwise_and(RHSV, Q, SUM);
        imshow("After ColourFilt", SUM);
        imshow("Original", imgOriginal);
        if (waitKey(30) == 'q') {
            Mat Calib1;
            Calib1 = SUM;
            Mat Check = SUM;
            outputQuad = contorsForCalib(Calib1, NforCalibr);
            if (outputQuad != 0) {
                cout << "outputQuad:" << endl;
                Point2f buf = outputQuad[0];
                outputQuad[0] = outputQuad[2];
                outputQuad[2] = buf;
                //
                buf = outputQuad[1];
                outputQuad[1] = outputQuad[2];
                outputQuad[2] = buf;

                for (int i = 0; i < NforCalibr; ++i) {
                    cout << outputQuad[i].x << ' ' << outputQuad[i].y << endl;
                }

                check(Check, outputQuad, NforCalibr);
                imshow("check", Check);
                imwrite("CheckCatch.png", Check);
            }
            else { cout << "Contours illegal"; }
        }
        if (MAP != 0) {
            if (CalStat == 0) {
                cout << endl;
                lambda = getPerspectiveTransform(inputQuad, outputQuad);
                lambdainv = lambda.inv();
                cout << lambdainv;
                cout << "Calibration has been done" << endl;
            }
            CalStat = 1;
        }
        if (CATCH != 0) {
            if (CheckStat == 0) {
                Mat ForCheckingCal;
                ForCheckingCal = imread("calibration.png");
                Point2f Pinv[NforCalibr];
                for (int i = 0; i < NforCalibr; i++) {

                    Pinv[i] = MatixVectorMult(LAMBDAinv, outputQuad[i]);
                }
                for (int i = 0; i < NforCalibr; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        cout << Pinv[i].x << ' ' << Pinv[i].y;
                    }
                    cout << endl;
                }
                Mat maz = imread("calibration.png");
                for (int i = 0; i < NforCalibr; ++i) {
                    circle(maz, Pinv[i], 8, Scalar(0, 0, 255), -1, 4);
                }
                imshow("Checking", maz);
                imwrite("Checking.png", maz);
            }
            CheckStat = 1;
        }

        if (waitKey(30) == 27) {
            writeinf(FileM, lambdainv);
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    return (0);
}
