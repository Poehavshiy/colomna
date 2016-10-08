//
// Created by nikita on 07.10.16.
//

#ifndef CALIBRATION_SMARTPOINT2F_H
#define CALIBRATION_SMARTPOINT2F_H
#include "My_exeptions.h"

class Smart_point2f {
    Point2f data;
public:
    const Point2f &getData() const {
        return data;
    }

public:


    Smart_point2f() : data() {

    }

    Smart_point2f(double x, double y) : data(x, y) {

    }

    bool friend operator<(const Smart_point2f &left,
                          const Smart_point2f &right) {
        if ( hypot(left.data.x, left.data.y) < hypot(right.data.x, right.data.y) )
            return true;

        return false;

    }
};


#endif //CALIBRATION_SMARTPOINT2F_H
