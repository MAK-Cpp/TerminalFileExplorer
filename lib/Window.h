#ifndef TERMINALFILEEXPLORER_LIB_WINDOW_H_
#define TERMINALFILEEXPLORER_LIB_WINDOW_H_

#include <memory>
#include <string>

#include "Point.h"
#include "ncurses.h"

enum class WindowSide { LEFT, TOP, RIGHT, BOTTOM, NONE };

enum class BorderState { NORMAL, BOLD };

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
    WINDOW* sub_window_;
    Point p0_;
    Point p1_;
    std::string name_;
    BorderState left_border_state_;
    BorderState right_border_state_;
    BorderState top_border_state_;
    BorderState bottom_border_state_;
    BorderState left_top_corner_state_;
    BorderState left_bottom_corner_state_;
    BorderState right_top_corner_state_;
    BorderState right_bottom_corner_state_;

    static int get_attributes(BorderState& state);
    void print_border();
    void print_content();
    void print_left_vertical_border();
    void print_right_vertical_border();
    void print_top_horizontal_border();
    void print_bottom_horizontal_border();
    void print_left_top_corner();
    void print_left_bottom_corner();
    void print_right_top_corner();
    void print_right_bottom_corner();

    static inline int horizontal_border_width   = 1;
    static inline int vertical_border_width     = 2;
    static inline std::string horizontal_border = "=";
    static inline std::string vertical_border   = "||";
    static inline std::string corner_border[]   = {"[]"};

public:
    Window(int nlines, int ncols, int y0, int x0, std::string name = "");
    operator WINDOW*() const;
    void set_name(std::string const& name);
    WindowSide get_side(int x, int y);
    [[nodiscard]] std::string name() const;
    [[nodiscard]] int get_ch() const;
    [[nodiscard]] Point const& p0() const;
    [[nodiscard]] Point const& p1() const;
    int width() const;
    int height() const;
    void print();
    void clear() const;
    void refresh();
    void set_keypad(int value);
    void move_x0(int l);
    void move_y0(int l);
    void move_x1(int l);
    void move_y1(int l);
    void stop_resize();
    ~Window();
};

#endif  // TERMINALFILEEXPLORER_LIB_WINDOW_H_
