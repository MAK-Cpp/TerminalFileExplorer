//
// Created by Maxim Primakov on 21.02.2024.
//

#include "WindowsHandler.h"

#include <utility>
WindowsHandler::WindowsHandler(): current_window_{-1} {
}
void WindowsHandler::add_window(int nlines, int ncols, int y0, int x0, std::string name, bool is_resizable) {
    windows_.emplace_back(new Window(nlines, ncols, y0, x0, std::move(name), is_resizable));
}
void WindowsHandler::add_terminal(int nlines, int ncols, int y0, int x0, std::string name, bool is_resizable) {
    windows_.emplace_back(new Terminal(nlines, ncols, y0, x0, std::move(name), is_resizable));
}
WindowsHandler::~WindowsHandler() {
    for (std::unique_ptr<Window> & ptr : windows_) {
        ptr.reset();
    }
}
void WindowsHandler::print() {
    for (std::unique_ptr<Window> const& ptr : windows_) {
        ptr->print();
    }
}
void WindowsHandler::set_current_window(int value) {
    current_window_ = value;
}
void WindowsHandler::set_current_window_keypad(int value) {
    windows_[current_window_]->set_keypad(value);
}
int WindowsHandler::get_current_window_ch() {
    return windows_[current_window_]->get_ch();
}
Window& WindowsHandler::get_window(int window_id) {
    return *windows_[window_id];
}
void WindowsHandler::start(int debug_window_id) {
    print();
    Window& debug = get_window(debug_window_id);
    bool while_state = true;
    int ch;
    bool is_resizing = false;
    WindowSide side  = WindowSide::NONE;
    int prev_mouse_x = -1;
    int prev_mouse_y = -1;

    while (while_state) {
        ch = get_current_window_ch();
        switch (ch) {
        case KEY_MOUSE: {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED & (~BUTTON1_PRESSED)) {
                    mvwprintw(debug, 6, 1, "left mouse button clicked!");
                    set_current_window_keypad(FALSE);
                    for (int i = 0; i < windows_.size(); ++i) {
                        if (wenclose(*windows_[i], event.y, event.x)) {
                            current_window_ = i;
                            break;
                        }
                    }
                    set_current_window_keypad(TRUE);
                } else if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
                    if (is_resizing) {
                        is_resizing  = false;
                        side         = WindowSide::NONE;
                        prev_mouse_x = -1;
                        prev_mouse_y = -1;
                        windows_[current_window_]->stop_resize();
                    } else {
                        side = windows_[current_window_]->get_side(event.x, event.y);
                        if ((is_resizing = (side != WindowSide::NONE))) {
                            prev_mouse_x = event.x;
                            prev_mouse_y = event.y;
                        }
                    }
                } else {
                    if (is_resizing) {
                        switch (side) {
                        case WindowSide::LEFT: {
                            windows_[current_window_]->move_x0(event.x - prev_mouse_x);
                            break;
                        }
                        case WindowSide::TOP: {
                            windows_[current_window_]->move_y0(event.y - prev_mouse_y);
                            break;
                        }
                        case WindowSide::RIGHT: {
                            mvwprintw(debug, 5, 1, "right");
                            windows_[current_window_]->move_x1(event.x - prev_mouse_x);
                            break;
                        }
                        case WindowSide::BOTTOM: {
                            mvwprintw(debug, 5, 1, "bottom");
                            windows_[current_window_]->move_y1(event.y - prev_mouse_y);
                            break;
                        }
                        case WindowSide::NONE: {
                            mvwprintw(debug, 5, 1, "NONE");
                            break;
                        }
                        }
                        prev_mouse_x = event.x;
                        prev_mouse_y = event.y;
                    }
                }
                mvwprintw(debug, 2, 1, "current_window window is %10s", windows_[current_window_]->name().c_str());
                mvwprintw(debug, 3, 1, "screen mouse coordinates is: %3d %3d", event.x, event.y);
                mvwprintw(debug, 6, 1, "event.bstate = %d", event.bstate);
                mvwprintw(debug, 7, 1, "event.bstate & BUTTON1_PRESSED = %d", event.bstate & BUTTON1_PRESSED);
                mvwprintw(debug, 8, 1, "event.bstate & BUTTON1_RELEASED = %d", event.bstate & BUTTON1_RELEASED);
                debug.refresh();
            }
            break;
        }
        case ctrl('C'): {
            while_state = false;
            break;
        }
        default: {
            windows_[current_window_]->key_capture(ch);
            break;
        }
        }
    }
}
