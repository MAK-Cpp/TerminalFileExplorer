#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#include "Directory.h"
#include "RasterLine.h"
#include "Window.h"
#include "ncurses.h"

namespace fs = std::filesystem;

#define ctrl(x) ((x) & 0x1f)

const std::string exit_message =
    R"(                                      +-----+     +----+
do you want to close file explorer?   | YES |     | NO |
                                      +-----+     +----+)";

/*
                                      +-----+     +----+
do you want to close file explorer?   | YES |     | NO |
                                      +-----+     +----+
 */

std::function<bool(Point)> check_mouse_in_square(Point &&a, Point &&b) {
    return [a, b](Point x) -> bool { return a.x() <= x.x() && a.y() <= x.y() && b.x() >= x.x() && b.y() >= x.y(); };
}

const int DEBUG_WIN_WIDTH = 80;

// void mouse_event(int ch, Window const windows[], int& current_window) {
//
// }

int main(int argc, char **argv) {
    initscr();

    Window debug(LINES / 2, DEBUG_WIN_WIDTH, 0, COLS - DEBUG_WIN_WIDTH, "DEBUG");
    Window win(LINES, COLS - DEBUG_WIN_WIDTH, 0, 0, "main window");

    Window windows[2] = {win, debug};

    // curs_set(0);  // hide cursor
    raw();        // disable line buffering
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    printf("\033[?1003h\n");  // Makes the terminal report mouse movement events
    std::atomic_bool while_state = true;
    int ch;
    win.print();
    debug.print();

    int current_window = 0;
    windows[current_window].set_keypad(TRUE);

    bool is_resizing = false;
    WindowSide side  = WindowSide::NONE;
    int prev_mouse_x = -1;
    int prev_mouse_y = -1;

    while (while_state) {
        ch = windows[current_window].get_ch();
        switch (ch) {
        case KEY_MOUSE: {
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON1_CLICKED & (~BUTTON1_PRESSED)) {
                    mvwprintw(debug, 6, 1, "left mouse button clicked!");
                    windows[current_window].set_keypad(FALSE);
                    if (wenclose(win, event.y, event.x)) {
                        current_window = 0;
                    } else if (wenclose(debug, event.y, event.x)) {
                        current_window = 1;
                    }
                    windows[current_window].set_keypad(TRUE);
                } else if (event.bstate & BUTTON1_DOUBLE_CLICKED) {
                    if (is_resizing) {
                        is_resizing  = false;
                        side         = WindowSide::NONE;
                        prev_mouse_x = -1;
                        prev_mouse_y = -1;
                        windows[current_window].stop_resize();
                    } else {
                        side = windows[current_window].get_side(event.x, event.y);
                        if ((is_resizing = (side != WindowSide::NONE))) {
                            prev_mouse_x = event.x;
                            prev_mouse_y = event.y;
                        }
                    }
                } else {
                    if (is_resizing) {
                        switch (side) {
                        case WindowSide::LEFT: {
                            windows[current_window].move_x0(event.x - prev_mouse_x);
                            break;
                        }
                        case WindowSide::TOP: {
                            windows[current_window].move_y0(event.y - prev_mouse_y);
                            break;
                        }
                        case WindowSide::RIGHT: {
                            mvwprintw(debug, 5, 1, "right");
                            windows[current_window].move_x1(event.x - prev_mouse_x);
                            break;
                        }
                        case WindowSide::BOTTOM: {
                            mvwprintw(debug, 5, 1, "bottom");
                            windows[current_window].move_y1(event.y - prev_mouse_y);
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
                mvwprintw(debug, 2, 1, "current_window window is %10s", windows[current_window].name().c_str());
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
            break;
        }
        }
    }
    printf("\033[?1003l\n");  // Disable mouse movement events, as l = low
    endwin();
    return 0;
}
