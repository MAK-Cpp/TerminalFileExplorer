#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>

#include "Directory.h"
#include "WindowsHandler.h"
#include "RasterLine.h"
#include "ncurses.h"

namespace fs = std::filesystem;


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

int main(int argc, char **argv) {
    initscr();

    WindowsHandler windows_handler;
    windows_handler.add_window(LINES, COLS / 2, 0, 0, "main window");
    windows_handler.add_window(LINES / 2, COLS - COLS / 2, 0, COLS - COLS / 2, "DEBUG", true);
    windows_handler.add_terminal(LINES - LINES / 2, COLS - COLS / 2, LINES - LINES / 2 - 1, COLS - COLS / 2, "TERMINAL", true);

    curs_set(0);  // hide cursor
    raw();        // disable line buffering
    noecho();
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    printf("\033[?1003h\n");  // Makes the terminal report mouse movement events
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    windows_handler.set_current_window(0);
    windows_handler.set_current_window_keypad(TRUE);

    windows_handler.start(1);

    printf("\033[?1003l\n");  // Disable mouse movement events, as l = low
    endwin();
    return 0;
}
