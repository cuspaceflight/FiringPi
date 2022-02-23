#include <cursesw.h>
#include <iostream>
#include <ncursesw/curses.h>
#include <locale.h>
#include <chrono>
#include <string>
#include <thread>

#include "State.hpp"


bool SOLENOID[3]{0};

void reinitwin(WINDOW * win, int height, int width, int starty,int startx)
{
  mvwin(win, starty, startx);
  wresize(win, height, width);
  wclear(win);
  box(win,0,0);
  wrefresh(win);
}

void draw_state(WINDOW *win, StateMachine machine)
{
  attron(COLOR_PAIR(machine.colors[machine.state])|A_REVERSE);
  mvprintw(3,5,"                ");
  mvprintw(4,5," CURRENT STATE  ");
  mvprintw(5,5,"                ");
  attroff(COLOR_PAIR(machine.colors[machine.state])|A_REVERSE);
  for (int i=0; auto state_name: machine.names) 
  {
    if (machine.state==i) { attron(A_REVERSE); }  
    else if (!machine.canChangeTo((State)i)) { attron(A_DIM); }
    mvprintw(2*i+7, 7, state_name);
    i++;
    attroff(A_REVERSE|A_DIM);
  };
}

void init_ncurses()
{
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  noecho();
  cbreak();
  curs_set(FALSE);
  nodelay(stdscr,TRUE);
  keypad(stdscr, TRUE);
  refresh();

  for (int i=0; i<COLORS && i<COLOR_PAIRS; i++)
  {
    init_pair(i, i, COLOR_BLACK);
  }
}

void draw_colors()
{
  for (int i=0; i<COLORS; i++)
  {
    attron(COLOR_PAIR(i));
    mvprintw(15+(i%12), 30+4*(i/12), "%d", i);
    attroff(COLOR_PAIR(i));
  }
}

int main()
{
  // systems stuff
  StateMachine machine;
  
  // ncurses stuff
  init_ncurses();
  WINDOW *main_win, *state_win, *valves_win;

  main_win = newwin(0,0,1,1);
  state_win = newwin(0,0,2,24);
  valves_win = newwin(0,0,2,3);

  ungetch(KEY_RESIZE);

  bool exit {};
  int ch;

  int diff {};
  float fps {};
  int stabilizer {16667};
  auto this_time = std::chrono::system_clock::now();
  auto last_time = this_time;

  const char* hint = "PRESS s[afe] OR q[uit]";
  cchar_t space {};
  setcchar(&space, L" ", 0, 0, (void*)0);

  while(!exit)
  {
    this_time = std::chrono::system_clock::now();
    diff = 0.8*diff+0.2*std::chrono::duration_cast<std::chrono::microseconds>(this_time-last_time).count();
    last_time = this_time;  
    fps=1000000.0/diff;
    stabilizer += 0.1*(16667-diff); 

    ch = getch();
    switch (ch)
    {
      case -1: break;
      case 'q':
        if (machine.state==OFF) { exit = true; }
        hint = "MUST BE POWERED OFF TO EXIT: ENTER s[afe] STATE THEN PRESS o[ff], q[uit]";
        break;
      case KEY_RESIZE:
        wclear(stdscr);
        refresh();
        //        window      height    width     y x 
        reinitwin(main_win,   LINES-2,  COLS-2,   1,1);
        reinitwin(state_win,  10,       COLS-27,  2,24);
        reinitwin(valves_win, LINES-4,  20,       2,3);
        break;
      default:
        machine.update(ch);
    }
    machine.process();
    
    draw_state(state_win,machine);
    // draw_colors();
    
    mvhline_set(0, 0, &space, COLS-10); 

    attron(COLOR_PAIR(3));
    mvprintw(0, 2, hint);
    if (ch>=0) { mvhline_set(0, COLS-10, &space, 10); mvprintw(0, COLS-10, "%d", ch);}
    mvprintw(0, COLS-20, "%.2f", fps);
    std::this_thread::sleep_for(std::chrono::microseconds(stabilizer));
    attroff(A_COLOR);
  }
  endwin();  
  return 0;
}


