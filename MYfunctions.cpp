#include "MYfunctions.h"

using namespace std;
int iLowH = 0;
int iHighH = 255;
int iLowS = 0;
int iHighS = 255;
int iLowV = 0;
int iHighV = 255;
int CATCH = 0;
int MAP = 0;

double FindMaks(Mat *A) {
    double MaksValue[2] = {1, 0};
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            Vec3b intensity = A->at<Vec3b>(i, j);
            MaksValue[1] = intensity.val[2];
            if (MaksValue[1] > MaksValue[0]) {
                MaksValue[0] = MaksValue[1];
            }
        }
    }
    return MaksValue[0];
}

double FindAverage(Mat *A) {
    double AverageValue = 0;
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            Vec3b intensity = A->at<Vec3b>(i, j);
            AverageValue += intensity.val[2];
        }
    }
    return AverageValue / (A->rows * A->cols);
}

std::string getImageType(int number) {
    int imgTypeInt = number % 8;
    std::string imgTypeString;

    switch (imgTypeInt) {
        case 0:
            imgTypeString = "8U";
            break;
        case 1:
            imgTypeString = "8S";
            break;
        case 2:
            imgTypeString = "16U";
            break;
        case 3:
            imgTypeString = "16S";
            break;
        case 4:
            imgTypeString = "32S";
            break;
        case 5:
            imgTypeString = "32F";
            break;
        case 6:
            imgTypeString = "64F";
            break;
        default:
            break;
    }

    // find channel
    int channel = (number / 8) + 1;

    std::stringstream type;
    type << "CV_" << imgTypeString << "C" << channel;

    return type.str();
}

int NumnonZEROPIX(Mat *A) {
    int PixelV, C = 0;
    for (int i = 0; i < A->rows; ++i) {
        for (int j = 0; j < A->cols; ++j) {
            PixelV = A->at<uchar>(i, j);
            if (PixelV != 0) {
                C++;
            }
        }
    }
    return C;
}

void histogr(Mat *src, Mat *dst) {
    vector<Mat> chanels(3);
    split(*src, chanels);
    equalizeHist(chanels[0], chanels[0]);
    equalizeHist(chanels[1], chanels[1]);
    equalizeHist(chanels[2], chanels[2]);
    merge(chanels, *dst);
    GaussianBlur(*dst, *dst, Size(9, 9), 9, 0);
}

void ColourFilt(Mat *src, Mat *dst) {
    GaussianBlur(*src, *dst, Size(9, 9), 9, 0);
    cvtColor(*dst, *dst, COLOR_BGR2HSV);//Hist � HSV
    vector<Mat> chanels(3);
    vector<Mat> mask(3);
    split(*dst, chanels);
    threshold(chanels[0], mask[0], iLowH, 1, CV_THRESH_BINARY);
    threshold(chanels[0], mask[1], iHighH, 1, CV_THRESH_BINARY_INV);
    multiply(mask[0], mask[1], chanels[0], 1);

    threshold(chanels[1], mask[0], iLowS, 1, CV_THRESH_BINARY);
    threshold(chanels[1], mask[1], iHighS, 1, CV_THRESH_BINARY_INV);
    multiply(mask[0], mask[1], chanels[1], 1);

    threshold(chanels[2], mask[0], iLowV, 1, CV_THRESH_BINARY);
    threshold(chanels[2], mask[1], iHighV, 1, CV_THRESH_BINARY_INV);
    multiply(mask[0], mask[1], chanels[2], 1);

    multiply(chanels[0], chanels[1], mask[2], 1);
    multiply(chanels[2], mask[2], mask[2], 1);

    bitwise_not(mask[2], mask[2]);
    cvtColor(*dst, *dst, CV_HSV2BGR);
    //*dst=*src ;
    //cvSet(dst,CV_RGB(0,0,0),mask[2]) ;
    cout << mask[2];
    cvtColor(mask[2], mask[2], CV_GRAY2RGB);
    imshow("mask[2]", mask[2]);
    bitwise_and(*dst, mask[2], *dst);
}

void TrackBar(int &iLowH, int &iHighH, int &iLowS, int &iHighS, int &iLowV, int &iHighV) {
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 255);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

void TrackBarBool(int &CATCH, int &MAP) {
    namedWindow("CATCH@MAP", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 500, 350);
    cvCreateTrackbar("CATCH", "CATCH@MAP", &CATCH, 1); //Hue (0 - 179)
    cvCreateTrackbar("MAP", "CATCH@MAP", &MAP, 1);
}

Point contors(Mat *Filtered) {
    int smallest_area = 0;
    int smallest_contour_index = 0;
    Point2f P;
    float Radius;
    cvtColor(*Filtered, *Filtered, CV_BGR2GRAY);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Moments mu;
    findContours(*Filtered, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < contours.size(); i++) {

        double a = contourArea(contours[i], false);
        if (a < smallest_area) {
            smallest_area = a;//cout<<i<<" area  "<<a<<endl;

            smallest_contour_index = i;
        }
    }

    if (contours.size() == NULL) {
        Point P;
        P.x = 0;
        P.y = 0;
        return P;
    }

    mu = moments(contours[smallest_contour_index], false);
    P = Point(mu.m10 / mu.m00, mu.m01 / mu.m00);
    return P;
}

Point2f *contorsForCalib(Mat Filtered, int Number) {
    Point2f *P;
    P = new Point2f[Number];
    cvtColor(Filtered, Filtered, CV_BGR2GRAY);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Moments mu;
    findContours(Filtered, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if (contours.size() == NULL) {
        cout << "Countors have not been found" << endl;
        return 0;
    }
    if (contours.size() < Number) {
        cout << "Too few contours" << endl;
        return 0;
    }

    for (int i = 0; i < Number; i++) {
        mu = moments(contours[i], false);
        P[i] = Point(mu.m10 / mu.m00, mu.m01 / mu.m00);
    }
    return P;
}

void Create_windows() {

    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    moveWindow("Original", 0, 0);

    namedWindow("After ColourFilt", CV_WINDOW_AUTOSIZE);
    moveWindow("After ColourFilt", 700, 0);

    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 350, 350);

}

Mat getMfrFILE(const char *adress) {
    ifstream is(adress);
    vector<double> a((istream_iterator<double>(is)), istream_iterator<double>());
    cout << endl;
    int k = 0, j = 0;
    Mat M(3, 3, CV_64F);
    for (int i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            M.at<double>(i, j) = a.at(k + j);
        }
        k += j;
    }
    is.close();
    return M;
}

Point2f MatixVectorMult(const Mat *oper, Point2f P) {
    Point2f res;
    res.x = (oper->at<double>(0, 0) * P.x + oper->at<double>(0, 1) * P.y + oper->at<double>(0, 2)) /
            (oper->at<double>(2, 0) * P.x + oper->at<double>(2, 1) * P.y + oper->at<double>(2, 2));

    res.y = (oper->at<double>(1, 0) * P.x + oper->at<double>(1, 1) * P.y + oper->at<double>(1, 2)) /
            (oper->at<double>(2, 0) * P.x + oper->at<double>(2, 1) * P.y + oper->at<double>(2, 2));
    return res;
}

void check(Mat A, Point2f *inputQuad, int Number) {
    string *s; /*= {"1", "2", "3", "4", "5", "6","7", "8", "9", "10", "11", "12" };*/
    s = new string[Number];
    int k = 49;
    int j = 48;
    for (int i = 0; i < Number; ++i, ++k) {
        if (i > 8) {
            k = 49;
            s[i].push_back(k);
            s[i].push_back(j);
            j++;
        }
        else { s[i].push_back(k); }
    }
    Mat check = A;
    for (int i = 0; i < Number; ++i) {
        circle(check, inputQuad[i], 8, Scalar(0, 255, 0), 0, 8);
    }

    for (int i = 0; i < Number; ++i) {

        putText(check, s[i], inputQuad[i], 1, 1, (0, 0, 255), 2, 4);
    }
}

void writeinf(FILE *f, Mat A) {
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            fprintf(f, "%2.10lf", A.at<double>(i, j));
            fprintf(f, "%c", ' ');
        }
        fprintf(f, "%c", '\n');

    }
}

void DirtySort(Point2f *outputQuad, int Number) {
    for (int i = Number - 1; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            if (outputQuad[j].y >= outputQuad[j + 1].y) {
                Point2f tmp = outputQuad[j];
                outputQuad[j] = outputQuad[j + 1];
                outputQuad[j + 1] = tmp;
            }
        }
    }
    for (int k = Number - 1; k >= 0; k = k - 5) {
        for (int i = k; i >= k - 4; i--) {
            for (int j = k - 4; j < i; j++) {
                if (outputQuad[j].x >= outputQuad[j + 1].x) {
                    Point2f tmp = outputQuad[j];
                    outputQuad[j] = outputQuad[j + 1];
                    outputQuad[j + 1] = tmp;
                }
            }
        }
    }
}

