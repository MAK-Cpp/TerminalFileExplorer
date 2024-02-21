//
// Created by Maxim Primakov on 21.02.2024.
//

#ifndef TERMINALFILEEXPLORER_LIB_WINDOWSHANDLER_H_
#define TERMINALFILEEXPLORER_LIB_WINDOWSHANDLER_H_

#define ctrl(x) ((x) & 0x1f)

#include "Window.h"
#include "Terminal.h"

class WindowsHandler {
private:
    std::vector<std::unique_ptr<Window>> windows_;
    int current_window_{};
public:
    WindowsHandler();
    void add_window(int nlines, int ncols, int y0, int x0, std::string name = "", bool is_resizable = false);
    void add_terminal(int nlines, int ncols, int y0, int x0, std::string name = "", bool is_resizable = false);
    void start(int debug_window_id);
    void set_current_window(int value);
    void set_current_window_keypad(int value);
    int get_current_window_ch();
    Window& get_window(int window_id);
    void print();
    ~WindowsHandler();
};

#endif  // TERMINALFILEEXPLORER_LIB_WINDOWSHANDLER_H_
