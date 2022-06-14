#include <Display.hpp>
#include <chrono>
#include <cstdlib>


Display::Display(
        std::shared_ptr<StateMachine> machine,
        std::shared_ptr<Relay> relays,
        std::shared_ptr<std::vector<PT *>> PTs,
        std::shared_ptr<std::vector<LoadCell *>> LCs,
        std::shared_ptr<std::vector<ADC *>> ADCs,
        std::shared_ptr<Logger> logger
) :
        open(true), graph_count(0), graph_interval(1), machine(machine), relays(relays),
        LCs(LCs), PTs(PTs), ADCs(ADCs), logger(logger) {


    setlocale(LC_ALL, "");
    initscr();
    start_color();
    noecho();
    cbreak();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    refresh();

    for (short i = 0; i < COLORS && i < COLOR_PAIRS; i++) {
        init_pair(i, i, COLOR_BLACK);
    }
    main_win = newwin(0, 0, 1, 1);
    top_win = newwin(0, 0, 2, 24);
    left_win = newwin(0, 0, 2, 3);

    graphs[0] = newwin((LINES - 14) / 2, (COLS - 27) / 2, 12, 24);
    graphs[1] = newwin((LINES - 14) / 2, (COLS - 27) / 2, 12, 24 + (COLS - 25) / 2);
    graphs[2] = newwin((LINES - 14) / 2, (COLS - 27) / 2, 12 + (LINES - 14) / 2, 24);
    graphs[3] = newwin((LINES - 14) / 2, (COLS - 27) / 2, 12 + (LINES - 14) / 2, 24 + (COLS - 25) / 2);

    // each source requires a pointer to an arbitrary float (can be a class member)
    // examples:
    // &((*PTs)[0]->*(&PT::pressure)) references the pressure of PT0
    // &(((*ADCs)[0]->*(&ADC::values))[0]) references value 0 of ADC0
    graph_srcs[0] = &(((*ADCs)[0]->*(&ADC::values))[0]);
    graph_srcs[1] = &(((*ADCs)[0]->*(&ADC::values))[1]);
    graph_srcs[2] = &(((*ADCs)[0]->*(&ADC::values))[2]);
    graph_srcs[3] = &((*LCs)[0]->*(&LoadCell::weight));

    for (int i = 0; i < 4; i++) {
        std::deque<float> temp{0};
        this->graph_bufs.push_back(temp);
    }

    ungetch(KEY_RESIZE);
    hint = "PRESS s[afe] OR q[uit]";
    setcchar(&space, L" ", 0, 0, nullptr);

    now = std::chrono::system_clock::now();
    last = now;

    update(true);

}

void Display::update(bool update_now) {
    now = std::chrono::system_clock::now();
    long diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count();
    if (diff < target_diff && !update_now) { return; }
    last = std::chrono::system_clock::now();

    ch = getch();

    switch (ch) {
        case -1:
            break;
        case 'q':
            if (machine->state == OFF) {
                logger->logging = false;
                logger->thread_obj->join();
                for (auto *pt: *PTs) {
                    pt->is_alive = false;
                    pt->thread_obj->join();
                }
                for (auto *lc: *LCs) {
                    lc->kill();
                }
                endwin();
                exit(0);
            }
            hint = "MUST BE POWERED OFF TO EXIT: ENTER s[afe] STATE THEN PRESS o[ff], q[uit]";
            break;
        case KEY_RESIZE:
            wclear(stdscr);
            refresh();
            //        window      height    width     y x
            reinitwin(main_win, LINES - 2, COLS - 2, 1, 1);
            reinitwin(top_win, 10, COLS - 27, 2, 24);
            reinitwin(left_win, LINES - 4, 20, 2, 3);

            reinitwin(graphs[0], (LINES - 14) / 2, (COLS - 27) / 2, 12, 24);
            reinitwin(graphs[1], (LINES - 14) / 2, (COLS - 27) / 2, 12, 24 + (COLS - 25) / 2);
            reinitwin(graphs[2], (LINES - 14) / 2, (COLS - 27) / 2, 12 + (LINES - 14) / 2, 24);
            reinitwin(graphs[3], (LINES - 14) / 2, (COLS - 27) / 2, 12 + (LINES - 14) / 2, 24 + (COLS - 25) / 2);
            graph_count=0;
            break;
        default:
            machine->update(ch);
            break;
    }

    draw_state();
    draw_gauges();
    if (graph_count==graph_interval) {
        for (int i = 0; i < 4; i++) draw_graphs(i);
        graph_count=0;
    }
    graph_count++;

    mvhline_set(0, 0, &space, COLS - 10);

    attron(COLOR_PAIR(3));
    mvprintw(0, 2, hint.c_str());
    if (ch >= 0) {
        mvhline_set(0, COLS - 10, &space, 10);
        mvprintw(0, COLS - 10, "%d", ch);
    }
    mvprintw(0, COLS - 25, "%.1f", 1000000.0f / (float) diff);
    attroff(A_COLOR);
}

void Display::reinitwin(WINDOW *win, int height, int width, int starty, int startx) {
    mvwin(win, starty, startx);
    wresize(win, height, width);
    wclear(win);
    box(win, 0, 0);
    wrefresh(win);
}

void Display::draw_state() {
    int color = StateMachine::colors[machine->state];
    wattron(left_win, COLOR_PAIR(color) | A_REVERSE);
    mvwprintw(left_win, 2, 2, "                ");
    mvwprintw(left_win, 3, 2, " CURRENT STATE  ");
    mvwprintw(left_win, 4, 2, "                ");
    wattroff(left_win, COLOR_PAIR(color) | A_REVERSE);
    for (int i = 0; auto state_name: StateMachine::names) {
        if (machine->state == i) { wattron(left_win, A_REVERSE); }
        else if (!machine->canChangeTo((State) i)) { wattron(left_win, A_DIM); }
        mvwprintw(left_win, 2 * i + 6, 4, state_name);
        i++;
        wattroff(left_win, A_REVERSE | A_DIM);
    }
    wrefresh(left_win);
}

void Display::draw_gauges() {
    mvwprintw(top_win, 1, 4, "P0: %f", (*PTs)[0]->pressure);
    mvwprintw(top_win, 2, 4, "T0: %f", (*PTs)[0]->temperature);

    mvwprintw(top_win, 4, 4, "P1: %f", (*PTs)[1]->pressure);
    mvwprintw(top_win, 5, 4, "T1: %f", (*PTs)[1]->temperature);

    mvwprintw(top_win, 7, 4, "P2: %f", (*PTs)[2]->pressure);
    mvwprintw(top_win, 8, 4, "T2: %f", (*PTs)[2]->temperature);

    mvwprintw(top_win, 1, 18, "LC0: %f", (*LCs)[0]->weight);


    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            mvwprintw(top_win, 5 + j, 18 + 20 * i, "ADC%d/%d: %5.0f", i, j, (*ADCs)[i]->values[j]);
        }
    }

    wrefresh(top_win);

}

void Display::draw_graphs(int i) {

    int lookup_l[]{0x40, 0x4, 0x2, 0x1};
    int lookup_r[]{0x80, 0x20, 0x10, 0x8};

    graph_bufs[i].push_back(*graph_srcs[i]);

    while ((int) graph_bufs[i].size() > 2 * (getmaxx(graphs[i]) - 1)) {
        graph_bufs[i].pop_front();
    }


    cchar_t *c;
    c->attr = COLOR_PAIR(2);
    int l, r, height;
    float pr, pl, max, min, scale;
    werase(graphs[i]);

    max = std::max(*std::max_element(graph_bufs[i].begin(), graph_bufs[i].end()), 0.0f);
    min = std::min(*std::min_element(graph_bufs[i].begin(), graph_bufs[i].end()), 0.0f);

    height = getmaxy(graphs[i]) - 2;
    scale = std::min((float) (height - 1) / (max - min), 50.0f);


    for (int j = 0; j < (int) graph_bufs[i].size()-1; j += 2) {
        pl = graph_bufs[i][j] - min;
        pr = graph_bufs[i][j + 1] - min;

        l = lookup_l[(int) std::fmod(pl / (0.25 / scale), 4)];
        r = lookup_r[(int) std::fmod(pr / (0.25 / scale), 4)];
        if (std::floor(scale * pl) == std::floor(scale * pr)) {
            *(c->chars) = 0x2800 + l + r;
            mvwadd_wch(graphs[i], height - (int) std::floor(scale * pl), 2 + j / 2, c);
        } else {
            *(c->chars) = 0x2800 + l;
            mvwadd_wch(graphs[i], height - (int) std::floor(scale * pl), 2 + j / 2, c);
            *(c->chars) = 0x2800 + r;
            mvwadd_wch(graphs[i], height - (int) std::floor(scale * pr), 2 + j / 2, c);
        }

    }
    mvwprintw(graphs[i], height - (int) std::floor(scale * (max - min)), 1, "%.2f", max);
    mvwprintw(graphs[i], height, 1, "%.2f", min);

    box(graphs[i], 0, 0);
    wrefresh(graphs[i]);

}

void Display::write_error(std::string message) {
    wattron(top_win, COLOR_PAIR(9));
    mvwprintw(top_win, 2, (COLS - 27) / 2, &message[0]);
    wattroff(top_win, COLOR_PAIR(9));
}