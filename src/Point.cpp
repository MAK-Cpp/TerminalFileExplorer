//
// Created by Maxim Primakov on 16.02.2024.
//

#include "Point.h"
int Point::x() const noexcept {
    return x_;
}
int Point::y() const noexcept {
    return y_;
}
Point::Point(int x, int y) : x_{x}, y_{y} {}
bool Point::operator==(const Point &other) const {
    return x_ == other.x_ && y_ == other.y_;
}
void Point::add_to_x(int v) {
    x_ += v;
}
void Point::add_to_y(int v) {
    y_ += v;
}
