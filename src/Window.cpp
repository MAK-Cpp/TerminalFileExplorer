//
// Created by Maxim Primakov on 17.02.2024.
//

#include "Window.h"

#include <utility>

Window::Window(int nlines, int ncols, int y0, int x0, std::string name)
    : window_{newwin(nlines, ncols, y0, x0)}
    , p0_{x0, y0}
    , p1_{x0 + ncols - 1, y0 + nlines - 1}
    , name_{std::move(name)}
    , sub_window_{subwin(window_, nlines - 2, ncols - 2, 1, 1)} {}
Window::~Window() {
    delwin(sub_window_);
    delwin(window_);
}
void Window::print() const {
    print_content();
    print_border();
    refresh();
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
    clear();
    p0_.add_to_x(l);
    wresize(window_, height(), width());
    mvwin(window_, p0_.y(), p0_.x());
    print();
    wrefresh(window_);
}
void Window::move_y0(int l) {
    clear();
    p0_.add_to_y(l);
    wresize(window_, height(), width());
    mvwin(window_, p0_.y(), p0_.x());
    print();
    wrefresh(window_);
}
void Window::move_x1(int l) {
    clear();
    p1_.add_to_x(l);
    wresize(window_, height(), width());
    print();
    wrefresh(window_);
}
void Window::move_y1(int l) {
    clear();
    p1_.add_to_y(l);
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
Point const& Window::p0() const {
    return p0_;
}
Point const& Window::p1() const {
    return p1_;
}
void Window::clear() const {
    wclear(window_);
    wrefresh(window_);
}
void Window::print_content() const {
    mvwprintw(sub_window_, 1, 1, "p0 = (%03d; %03d)", p0_.x(), p0_.y());
    mvwprintw(sub_window_, 2, 1, "p1 = (%03d; %03d)", p1_.x(), p1_.y());
}
void Window::print_border() const {
    const int name_size           = name_.size() + 4;  // name + 2 spaces + 2 brackets
    const int name_start_pos      = (width() - name_size + 1) / 2;
    const int name_end_pos        = name_start_pos + name_size - 1;
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
    mvwaddch(window_, 0, 0, '*');
    mvwaddch(window_, height() - 1, 0, '+');
    mvwaddch(window_, 0, width() - 1, '+');
    mvwaddch(window_, height() - 1, width() - 1, '*');
    // box(window_, 0, 0);
}
void Window::refresh() const {
    wrefresh(sub_window_);
    wrefresh(window_);
}
