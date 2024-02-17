#ifndef TERMINALFILEEXPLORER_LIB_RASTERLINE_H_
#define TERMINALFILEEXPLORER_LIB_RASTERLINE_H_

#include <vector>

#include "Point.h"
#include "ncurses.h"

class RasterLine {
private:
    std::vector<Point> points_;
    constexpr static inline double lambda_ = 3.125e-3;

public:
    RasterLine(int x1, int y1, int x2, int y2);
    void draw(WINDOW* const window);
    void clear(WINDOW* const window);
};

#endif  // TERMINALFILEEXPLORER_LIB_RASTERLINE_H_
