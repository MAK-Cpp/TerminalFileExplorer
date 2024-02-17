#include <filesystem>
#include <functional>
#include <iostream>
#include <string>

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

constexpr int DEBUG_WIN_WIDTH = 50;

int main(int argc, char **argv) {
    initscr();

    // WINDOW *const win   = newwin(LINES, COLS - DEBUG_WIN_WIDTH, 0, 0);
    // WINDOW *const debug = newwin(6, DEBUG_WIN_WIDTH - 1, 0, COLS - DEBUG_WIN_WIDTH);
    Window win(LINES - 6, COLS - DEBUG_WIN_WIDTH, 0, 0, "main window");
    // Window win(3, 4, 0, 0);
    Window debug(12, DEBUG_WIN_WIDTH / 1.5, 0, COLS - (DEBUG_WIN_WIDTH / 1.5), "DEBUG");

    Window windows[2] = {win, debug};

    curs_set(0);  // hide cursor
    raw();        // disable line buffering
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    printf("\033[?1003h\n");  // Makes the terminal report mouse movement events
    bool while_state = true;
    int ch;
    debug.print();
    win.print();

    int current_window = 0;
    keypad(windows[current_window], TRUE);

    mvwprintw(debug, 1, 1, "current_window window is %10s", windows[current_window].name().c_str());
    wrefresh(debug);

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
                    mvwprintw(debug, 5, 1, "left mouse button clicked!");
                    keypad(windows[current_window], FALSE);
                    if (wenclose(win, event.y, event.x)) {
                        current_window = 0;
                    } else if (wenclose(debug, event.y, event.x)) {
                        current_window = 1;
                    }
                    keypad(windows[current_window], TRUE);
                } else if (event.bstate & BUTTON1_PRESSED) {
                    side = windows[current_window].get_side(event.x, event.y);
                    prev_mouse_x = event.x;
                    prev_mouse_y = event.y;
                    if (side != WindowSide::NONE) {
                        is_resizing = true;
                    }
                } else if (event.bstate & BUTTON1_RELEASED) {
                    if (is_resizing) {
                        is_resizing  = false;
                        side = WindowSide::NONE;
                        prev_mouse_x = -1;
                        prev_mouse_y = -1;
                    }
                } else {
                    if (!is_resizing) {
                        wclear(debug);
                        debug.print();
                    } else {
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
                            windows[current_window].move_x1(event.x - prev_mouse_x);
                            break;
                        }
                        case WindowSide::BOTTOM: {
                            windows[current_window].move_y1(event.y - prev_mouse_y);
                            break;
                        }
                        case WindowSide::NONE: {
                            break;
                        }
                        }
                        prev_mouse_x = event.x;
                        prev_mouse_y = event.y;
                    }
                }
                mvwprintw(debug, 2, 1, "screen mouse coordinates is: %3d %3d", event.x, event.y);
                wmouse_trafo(windows[current_window], &event.y, &event.x, false);
                mvwprintw(debug, 3, 1, "current_window mouse coordinates is %3d %3d", event.x, event.y);
                mvwprintw(debug, 1, 1, "current_window window is %10s", windows[current_window].name().c_str());
                wrefresh(debug);
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
    /*
    while (while_state) {
        ch = getch();
        switch (ch) {
        case ctrl('L'): {
            fs::path path     = argv[0];
            fs::path abs_path = fs::absolute(path).parent_path();
            for (const auto &entry : fs::directory_iterator(abs_path)) {
                wprintw(win, (entry.path().filename().string() + '\n').c_str());
            }
            wrefresh(win);
            break;
        }
        case ctrl('D'): {
            line.draw(win);
            break;
        }
        case ctrl('F'): {
            line.clear(win);
            break;
        }
        case ctrl('C'): {
            std::function<bool(Point)> yes_button = check_mouse_in_square(Point(38, LINES - 3), Point(44, LINES - 1));
            mvwprintw(win, LINES - 3, 0, exit_message.c_str());
            wrefresh(win);
            while (true) {
                ch = getch();
                if (ch == KEY_MOUSE) {
                    if (getmouse(&event) == OK) {
                        mvwprintw(debug, 1, 0, "DEBUG:\nmouse last click position:\nx = %3d\ny = %3d", event.x,
                                  event.y);
                        wrefresh(debug);
                        if ((event.bstate & BUTTON1_CLICKED) && yes_button(Point(event.x, event.y))) {
                            break;
                        }
                    }
                }
            }
            while_state = !yes_button(Point(event.x, event.y));
            break;
        }
        case KEY_BACKSPACE:
        case 127:
        case '\b': {
            // key_backspace
            int x, y;
            getyx(win, y, x);
            if (x > 0) {
                --x;
            } else if (y > 0) {
            }
            wmove(win, y, x);
            wdelch(win);
            wrefresh(win);
            break;
        }
        default: {
            waddch(win, ch);
            wrefresh(win);
            break;
        }
        }
    }
    */
    printf("\033[?1003l\n");  // Disable mouse movement events, as l = low
    endwin();
    return 0;
}
