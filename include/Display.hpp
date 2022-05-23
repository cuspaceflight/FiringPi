#pragma once

#include <cursesw.h>
#include <iostream>
#include <ncursesw/curses.h>
#include <string>
#include <chrono>
#include <vector>
#include <deque>
#include <cmath>

#include "State.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "LoadCell.hpp"


class Display {
public:
    Display(StateMachine *statemachine, Relay *relays, std::vector<PT*> *pts, std::shared_ptr<LoadCell> load_cell);

    void update();

private:
    int ch;
    std::string hint;
    std::deque<float> graph_buffer;
    cchar_t space;
    WINDOW *main_win, *top_win, *left_win, *graph_win;
    StateMachine *machine;
    Relay *relays;
    std::shared_ptr<LoadCell> load_cell;
    std::vector<PT*> *pts;
    std::chrono::time_point<std::chrono::system_clock> now, last;

    static const long target_diff = 1000000/60;

    static void reinitwin(WINDOW *win, int height, int width, int starty, int startx);

    void draw_state();

    void draw_gauges();

    void draw_graphs();

};
