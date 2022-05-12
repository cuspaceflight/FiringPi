#include <Display.hpp>
#include <chrono>
#include <cstdlib>


Display::Display(StateMachine *machine, Relay *relays, std::vector<PT *> *pts, Logger *logger) {
    this->machine = machine;
    this->relays = relays;
    this->pts = pts;
    this->logger = logger;
    this->open = true;
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
    graph_win = newwin(0, 0, 12, 25);

    ungetch(KEY_RESIZE);
    hint = "PRESS s[afe] OR q[uit]";
    setcchar(&space, L" ", 0, 0, nullptr);

    now = std::chrono::system_clock::now();
    last = now;

}

void Display::update() {
    now = std::chrono::system_clock::now();
    long diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count();
    if (diff < target_diff) { return; }
    last = std::chrono::system_clock::now();

    ch = getch();

    switch (ch) {
        case -1:
            break;
        case 'q':
            if (machine->state == OFF) {
                logger->logging = false;
                logger->thread_obj->join();
                for (auto *pt: *pts) {
                    pt->is_alive = false;
                    pt->thread_obj->join();
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
            reinitwin(graph_win, (LINES - 14)/2, (COLS - 27)/2, 12, 24);
            break;
        default:
            machine->update(ch);
            break;
    }

    draw_state();
    draw_gauges();
    draw_graphs();

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
    mvwprintw(top_win, 4, 4, "P: %f", (*pts)[0]->pressure());
    mvwprintw(top_win, 5, 4, "T: %f", (*pts)[0]->temperature());
    wrefresh(top_win);
}

void Display::draw_graphs() {

    graph_buffer.push_back((*pts)[0]->pressure());
    while (graph_buffer.size()>2*(getmaxx(graph_win)-1)) { graph_buffer.pop_front(); }

    int lookup_l[]{0x40, 0x4, 0x2, 0x1};
    int lookup_r[]{0x80, 0x20, 0x10, 0x8};

    cchar_t *c;
    c->attr = COLOR_PAIR(2);
    int l, r;
    float pr, pl, scale{10};


    werase(graph_win);
    for (int i = 0; i < graph_buffer.size(); i += 2) {
        pr = 1+graph_buffer[i];
        pl = 1+graph_buffer[i + 1];
        l = lookup_l[(int) std::fmod(pl / (0.25 / scale), 4)];
        r = lookup_r[(int) std::fmod(pr / (0.25 / scale), 4)];
        if (std::fmod(pl,1/scale)==std::fmod(pr,1/scale)) {
            *(c->chars) = 0x2800 + l + r;
            mvwadd_wch(graph_win, 7-(int) std::floor(scale * (pl-1)), 2 + i / 2, c);
        } else {
            *(c->chars) = 0x2800 + l;
            mvwadd_wch(graph_win, 7-(int) std::floor(scale * (pl-1)), 2 + i / 2, c);
            *(c->chars) = 0x2800 + r;
            mvwadd_wch(graph_win, 7-(int) std::floor(scale * (pr-1)), 2 + i / 2, c);
        }

    }
    box(graph_win,0,0);
    wrefresh(graph_win);

}