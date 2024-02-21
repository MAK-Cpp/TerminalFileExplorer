//
// Created by Maxim Primakov on 20.02.2024.
//

#ifndef TERMINALFILEEXPLORER_LIB_TERMINAL_H_
#define TERMINALFILEEXPLORER_LIB_TERMINAL_H_

#include "Window.h"

class Terminal : public Window {
protected:
    void print_content() override;
    void add_cursor();
    void delete_cursor();
public:
    Terminal(int nlines, int ncols, int y0, int x0, std::string name = "", bool is_resizable = false);
    void key_capture(int ch) override;
    void set_keypad(int value) override;
    ~Terminal() override;
};

#endif  // TERMINALFILEEXPLORER_LIB_TERMINAL_H_
