//
// Created by Maxim Primakov on 17.02.2024.
//

#include "Window.h"
Window::Window(int nlines, int ncols, int y0, int x0, std::string const& name)
    : window_{newwin(nlines, ncols, y0, x0)}, p0_{x0, y0}, p1_{x0 + ncols - 1, y0 + nlines - 1}, name_{name} {}
Window::~Window() {
    delwin(window_);
}
void Window::print() const {
    const int name_size = name_.size() + 4; // name + 2 spaces + 2 brackets
    const int name_start_pos = (width() - name_size + 1) / 2;
    const int name_end_pos = name_start_pos + name_size - 1;
    const std::string name_result = "[ " + name_ + " ]";
    for (int i = 1; i < width() - 1; ++i) {
        if (name_start_pos <= i && i <= name_end_pos) {
            mvwaddch(window_, 0, i, name_result[i - name_start_pos]);
        } else {
            mvwaddch(window_, 0, i, '-');
        }
        mvwaddch(window_, height() - 1, i, '-');
    }
    for (int j = 1; j < height() - 1; ++j) {
        mvwaddch(window_, j, 0, '|');
        mvwaddch(window_, j, width() - 1, '|');
    }
    mvwaddch(window_, 0, 0, '+');
    mvwaddch(window_, height() - 1, 0, '+');
    mvwaddch(window_, 0, width() - 1, '+');
    mvwaddch(window_, height() - 1, width() - 1, '+');
    wmove(window_, 1, 1);
    // box(window_, 0, 0);
    wrefresh(window_);
}
int Window::get_ch() const {
    return wgetch(window_);
}
Window::operator WINDOW*() const {
    return window_;
}
void Window::set_name(std::string const& name) {
    name_ = name;
}
std::string Window::name() const {
    return name_;
}
WindowSide Window::get_side(int x, int y) {
    int delta = 1;
    if (p0_.y() - delta <= y && y <= p0_.y() + delta && p0_.x() <= x && x <= p1_.x()) {
        return WindowSide::TOP;
    } else if (p1_.y() - delta <= y && y <= p1_.y() + delta && p0_.x() <= x && x <= p1_.x()) {
        return WindowSide::BOTTOM;
    } else if (p0_.x() - delta <= x && x <= p0_.x() + delta && p0_.y() <= y && y <= p1_.y()) {
        return WindowSide::LEFT;
    } else if (p1_.x() - delta <= x && x <= p1_.x() + delta && p0_.y() <= y && y <= p1_.y()) {
        return WindowSide::RIGHT;
    }
    return WindowSide::NONE;
}
void Window::move_x0(int l) {
    wclear(window_);
    wrefresh(window_);
    p0_.add_to_x(l);
    mvwin(window_, p0_.y(), p0_.x());
    wresize(window_, height(), width());
    print();
    wrefresh(window_);
}
void Window::move_y0(int l) {
    wclear(window_);
    wrefresh(window_);
    p0_.add_to_y(l);
    mvwin(window_, p0_.y(), p0_.x());
    wresize(window_, height(), width());
    print();
    wrefresh(window_);
}
void Window::move_x1(int l) {
    wclear(window_);
    wrefresh(window_);
    p1_.add_to_x(l);
    mvwin(window_, p0_.y(), p0_.x());
    wresize(window_, height(), width());
    print();
    wrefresh(window_);
}
void Window::move_y1(int l) {
    wclear(window_);
    wrefresh(window_);
    p1_.add_to_y(l);
    mvwin(window_, p0_.y(), p0_.x());
    wresize(window_, height(), width());
    print();
    wrefresh(window_);
}
int Window::width() const {
    return p1_.x() - p0_.x() + 1;
}
int Window::height() const {
    return p1_.y() - p0_.y() + 1;
}
