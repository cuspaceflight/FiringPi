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
#include "Logger.hpp"
#include "LoadCell.hpp"


class Display {
public:
    Display(
            std::shared_ptr<StateMachine> machine,
            std::shared_ptr<Relay> relays,
            std::shared_ptr<std::vector<PT*>> pts,
            std::shared_ptr<LoadCell> load_cell,
            std::shared_ptr<Logger> logger
    );

    void update(bool update_now = false);
    bool open;
    void write_error(std::string message);

private:
    int ch;
    std::string hint;
    std::deque<float> graph_buffer;
    cchar_t space;
    WINDOW *main_win, *top_win, *left_win, *graph_win;
    std::shared_ptr<StateMachine> machine;
    std::shared_ptr<Relay> relays;
    std::shared_ptr<LoadCell> load_cell;
    std::shared_ptr<std::vector<PT*>> pts;
    std::shared_ptr<Logger> logger;
    std::chrono::time_point<std::chrono::system_clock> now, last;

    static const long target_diff = 1000000/60;

    static void reinitwin(WINDOW *win, int height, int width, int starty, int startx);

    void draw_state();

    void draw_gauges();

    void draw_graphs();

};
