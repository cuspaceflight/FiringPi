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
#include "ADC.hpp"


class Display {
public:
    Display(
            std::shared_ptr<StateMachine> machine,
            std::shared_ptr<Relay> relays,
            std::shared_ptr<std::vector<PT *>> PTs,
            std::shared_ptr<std::vector<LoadCell *>> LCs,
            std::shared_ptr<std::vector<ADC *>> ADCs,
            std::shared_ptr<Logger> logger
    );

    void update(bool update_now = false);

    bool open;
    int scr;

    void write_error(std::string message);

private:
    int ch;
    std::string hint;
    cchar_t space;

    std::vector<std::deque<float>> graph_bufs;
    WINDOW *main_win, *top_win, *left_win;
    std::array<WINDOW *,4> graphs;
    std::array<float *, 17> graph_srcs;
    int graph_count, graph_interval;

    std::shared_ptr<StateMachine> machine;
    std::shared_ptr<Relay> relays;
    std::shared_ptr<std::vector<LoadCell *>> LCs;
    std::shared_ptr<std::vector<PT *>> PTs;
    std::shared_ptr<std::vector<ADC *>> ADCs;
    std::shared_ptr<Logger> logger;

    std::chrono::time_point<std::chrono::system_clock> now, last;

    static const long target_diff = 1000000 / 60;

    static const int screens[][4];

    static void reinitwin(WINDOW *win, int height, int width, int starty, int startx);

    void draw_state();

    void draw_gauges();

    void draw_graphs(int i);

};
