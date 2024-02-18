//
// Created by Maxim Primakov on 17.02.2024.
//

#include "Window.h"

#include <utility>

// #define DEBUG_WIN_BOX

Window::Window(int nlines, int ncols, int y0, int x0, std::string name)
    : p0_{x0, y0}, p1_{x0 + ncols - 1, y0 + nlines - 1}, name_{std::move(name)} {
    this->window_ = newwin(nlines, ncols, y0, x0);
    this->sub_window_ =
        subwin(this->window_, nlines - (horizontal_border_width << 1), ncols - (vertical_border_width << 1),
               y0 + horizontal_border_width, x0 + vertical_border_width);
}
Window::~Window() {
    delwin(sub_window_);
    delwin(window_);
}
void Window::print() const {
    print_border();
    refresh();
    print_content();
    refresh();
}
int Window::get_ch() const {
    return wgetch(window_);
}
Window::operator WINDOW*() const {
    return sub_window_;
    // return window_;
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
    wresize(sub_window_, height() - 2, width() - 2);
    wresize(window_, height(), width());
    mvwin(sub_window_, p0_.y() + 1, p0_.x() + 1);
    mvwin(window_, p0_.y(), p0_.x());
    print();
    refresh();
}
void Window::move_y0(int l) {
    clear();
    p0_.add_to_y(l);
    wresize(sub_window_, height() - 2, width() - 2);
    wresize(window_, height(), width());
    mvwin(sub_window_, p0_.y() + 1, p0_.x() + 1);
    mvwin(window_, p0_.y(), p0_.x());
    print();
    refresh();
}
void Window::move_x1(int l) {
    clear();
    p1_.add_to_x(l);
    wresize(sub_window_, height() - 2, width() - 2);
    wresize(window_, height(), width());
    print();
    refresh();
}
void Window::move_y1(int l) {
    clear();
    p1_.add_to_y(l);
    wresize(sub_window_, height() - 2, width() - 2);
    wresize(window_, height(), width());
    print();
    refresh();
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
#ifdef DEBUG_WIN_BOX
    box(sub_window_, 0, 0);
#endif
}
void Window::print_border() const {
    const int name_size           = name_.size() + 4;  // name + 2 spaces + 2 brackets
    const int name_start_pos      = (width() - name_size + 1) / 2;
    const int name_end_pos        = name_start_pos + name_size - 1;
    const std::string name_result = "[ " + name_ + " ]";

    // horizontal border
    for (int i = vertical_border_width; i < width() - vertical_border_width; ++i) {
        for (int j = 0, dj = height() - horizontal_border_width; j < horizontal_border_width; ++j, ++dj) {
            if (name_start_pos <= i && i <= name_end_pos && j == horizontal_border_width / 2) {
                mvwaddch(window_, j, i, name_result[i - name_start_pos]);
            } else {
                mvwaddch(window_, j, i, horizontal_border[j]);
            }
            mvwaddch(window_, dj, i, horizontal_border[j]);
        }
    }

    // vertical border
    for (int j = horizontal_border_width; j < height() - horizontal_border_width; ++j) {
        for (int i = 0, di = width() - vertical_border_width; i < vertical_border_width; ++i, ++di) {
            mvwaddch(window_, j, i, vertical_border[i]);
            mvwaddch(window_, j, di, vertical_border[i]);
        }
    }

    // corners
    for (int j = 0, dj = height() - horizontal_border_width; j < horizontal_border_width; ++j, ++dj) {
        for (int i = 0, di = width() - vertical_border_width; i < vertical_border_width; ++i, ++di) {
            mvwaddch(window_, j, i, corner_border[j][i]);
            mvwaddch(window_, dj, i, corner_border[j][i]);
            mvwaddch(window_, j, di, corner_border[j][i]);
            mvwaddch(window_, dj, di, corner_border[j][i]);
        }
    }
#ifdef DEBUG_WIN_BOX
    box(window_, 0, 0);
#endif
}
void Window::refresh() const {
    wrefresh(sub_window_);
    wrefresh(window_);
}
void Window::set_keypad(int value) {
    keypad(window_, value);
}
