//
// Created by Maxim Primakov on 20.02.2024.
//

#include "Terminal.h"

#include <utility>
Terminal::Terminal(int nlines, int ncols, int y0, int x0, std::string name, bool is_resizable)
    : Window(nlines, ncols, y0, x0, std::move(name), is_resizable) {}
void Terminal::key_capture(int ch) {
    delete_cursor();
    switch (ch) {
    case KEY_ENTER: {
        waddch(sub_window_, '\n');
        break;
    }
    case KEY_BACKSPACE:
    case 127:
    case '\b': {
        wmove(sub_window_, getcury(sub_window_), getcurx(sub_window_) - 1);
        wdelch(sub_window_);
        break;
    }
    default: {
        waddch(sub_window_, ch);
        break;
    }
    }
    add_cursor();
    refresh();
}
Terminal::~Terminal() {

    Window::~Window();
}
void Terminal::print_content() {
}
void Terminal::add_cursor() {
    waddch(sub_window_, '|' | A_BLINK | A_BOLD);
}
void Terminal::delete_cursor() {
    wmove(sub_window_, getcury(sub_window_), getcurx(sub_window_) - 1);
    wdelch(sub_window_);
}
void Terminal::set_keypad(int value) {
    Window::set_keypad(value);
    if (is_chosen_) {
        add_cursor();
    } else {
        delete_cursor();
    }
    refresh();
}
