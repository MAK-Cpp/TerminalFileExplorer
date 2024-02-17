#include "RasterLine.h"

#include <chrono>
#include <cmath>
#include <string>
#include <thread>

using std::operator""s;

void RasterLine::draw(WINDOW* const window) {
    for (int i = 0; i < points_.size(); ++i) {
        Point const& p = points_[i];
        char result    = '.';
        if (i == 0 || i == points_.size()) {
            result = '*';
        } else {
            Point const& prev_p = points_[i - 1];
            Point const& next_p = points_[i + 1];
            if (prev_p == next_p || prev_p == p || p == next_p) {
                result = '*';
            } else {
            }
        }
        std::string const format = "i=%2d";
        mvwprintw(window, LINES - 1, COLS - format.size(), format.c_str(), i);
        mvwaddch(window, p.y(), p.x(), result);
        wrefresh(window);
        std::this_thread::sleep_for(1s);
    }
}

RasterLine::RasterLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    for (float l = 0.0; l < 1.0; l += lambda_) {
        int curr_x = x1 + int(std::floor(dx * l + 0.5));
        int curr_y = y1 + int(std::floor(dy * l + 0.5));
        Point another(curr_x, curr_y);
        if (points_.empty() || !(points_[points_.size() - 1] == another)) {
            points_.push_back(another);
        }
    }
}

void RasterLine::clear(WINDOW* const window) {
    for (Point const& p : points_) {
        mvwaddch(window, p.y(), p.x(), ' ');
    }
    wrefresh(window);
}
