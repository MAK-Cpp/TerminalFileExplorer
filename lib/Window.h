#ifndef TERMINALFILEEXPLORER_LIB_WINDOW_H_
#define TERMINALFILEEXPLORER_LIB_WINDOW_H_

#include <memory>
#include <string>

#include "Point.h"
#include "ncurses.h"

enum class WindowSide { LEFT, TOP, RIGHT, BOTTOM, NONE };

class Window {
    /*

 (x0, y0)
     +-----------[ WINDOW NAME ]------------+
     |                                      |
     |                                      |
     |                                      |
     |                                      |
     |                                      |
     |                                      |
     +--------------------------------------+
                                        (x1, y1)

     */
private:
    WINDOW* window_;
    Point p0_;
    Point p1_;
    std::string name_;

public:
    Window(int nlines, int ncols, int y0, int x0, std::string const& name = "");
    operator WINDOW*() const;
    void set_name(std::string const& name);
    WindowSide get_side(int x, int y);
    [[nodiscard]] std::string name() const;
    [[nodiscard]] int get_ch() const;
    int width() const;
    int height() const;
    void print() const;
    void move_x0(int l);
    void move_y0(int l);
    void move_x1(int l);
    void move_y1(int l);
    ~Window();
};

#endif  // TERMINALFILEEXPLORER_LIB_WINDOW_H_
