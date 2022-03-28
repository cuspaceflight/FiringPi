#include <Display.hpp>
#include <chrono>
#include <cstdlib>


Display::Display(StateMachine *machine, Relay *relays, std::vector<PT*> *pts) {
    this->machine = machine;
    this->relays = relays;
    this->pts = pts;
    this->open=true;
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
    state_win = newwin(0, 0, 2, 24);
    valves_win = newwin(0, 0, 2, 3);

    ungetch(KEY_RESIZE);
    hint = "PRESS s[afe] OR q[uit]";
    setcchar(&space, L" ", 0, 0, nullptr);

    now = std::chrono::system_clock::now();
    last = now;

}

void Display::update() {
    now = std::chrono::system_clock::now();
    long diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count();
    if (diff < 16667) { return; }
    last = std::chrono::system_clock::now();

    ch = getch();

    switch (ch) {
        case -1:
            break;
        case 'q':
            if (machine->state == OFF) {
                for (auto *pt: *pts) {pt->is_alive = false; pt->thread_obj->join();}
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
            reinitwin(state_win, 10, COLS - 27, 2, 24);
            reinitwin(valves_win, LINES - 4, 20, 2, 3);
            break;
        default:
            machine->update(ch);
            break;
    }
    draw_state();
    draw_gauges();
    // draw_colors();

    mvhline_set(0, 0, &space, COLS - 10);

    attron(COLOR_PAIR(3));
    mvprintw(0, 2, hint.c_str());
    if (ch >= 0) {
        mvhline_set(0, COLS - 10, &space, 10);
        mvprintw(0, COLS - 10, "%d", ch);
    }
    mvprintw(0, COLS - 25, "%.1f", 1000000.0f / (float)diff);
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
    attron(COLOR_PAIR(color) | A_REVERSE);
    mvprintw(3, 5, "                ");
    mvprintw(4, 5, " CURRENT STATE  ");
    mvprintw(5, 5, "                ");
    attroff(COLOR_PAIR(color) | A_REVERSE);
    for (int i = 0; auto state_name: StateMachine::names) {
        if (machine->state == i) { attron(A_REVERSE); }
        else if (!machine->canChangeTo((State) i)) { attron(A_DIM); }
        mvprintw(2 * i + 7, 7, state_name);
        i++;
        attroff(A_REVERSE | A_DIM);
    }
}

void Display::draw_gauges() {
    mvprintw(6,30,"P: %f",(*pts)[0]->pressure());
    mvprintw(7,30,"T: %f",(*pts)[0]->temperature());

    mvprintw(6,50,"P: %f",(*pts)[1]->pressure());
    mvprintw(7,50,"T: %f",(*pts)[1]->temperature());}