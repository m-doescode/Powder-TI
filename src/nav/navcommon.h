#pragma once

#include <stdint.h>

namespace nav {
    template <typename int_type>
    struct Point_r {
        int_type x;
        int_type y;

        inline Point_r(int_type x, int_type y) : x(x), y(y) {}


        Point_r<int_type> operator -() {
            return Point_r(-x, -y);
        }


        Point_r<int_type> operator +(int_type z) {
            return Point_r(x + z, y + z);
        }

        Point_r<int_type> operator +(Point_r<int_type> z) {
            return Point_r(x + z.x, y + z.y);
        };

        Point_r<int_type> operator +=(int_type z) {
            return *this = *this + z;
        }

        Point_r<int_type> operator +=(Point_r<int_type> z) {
            return *this = *this + z;
        }


        Point_r<int_type> operator -(int_type z) {
            return *this + -z;
        }

        Point_r<int_type> operator -(Point_r<int_type> z) {
            return *this + -z;
        }

        Point_r<int_type> operator -=(int_type z) {
            return *this = *this - z;
        }

        Point_r<int_type> operator -=(Point_r<int_type> z) {
            return *this = *this - z;
        }
        

        Point_r<int_type> operator *(int_type z) {
            return Point_r(x * z, y * z);
        }

        Point_r<int_type> operator *(Point_r<int_type> z) {
            return Point_r(x * z.x, y * z.y);
        };

        Point_r<int_type> operator *=(int_type z) {
            return *this = *this * z;
        }

        Point_r<int_type> operator *=(Point_r<int_type> z) {
            return *this = *this * z;
        }
        

        Point_r<int_type> operator /(int_type z) {
            return Point_r(x / z, y / z);
        }

        Point_r<int_type> operator /(Point_r<int_type> z) {
            return Point_r(x / z.x, y / z.y);
        }

        Point_r<int_type> operator /=(int_type z) {
            return *this = *this / z;
        }

        Point_r<int_type> operator /=(Point_r<int_type> z) {
            return *this = *this / z;
        }
    };

    typedef Point_r<uint8_t> Point8;
}