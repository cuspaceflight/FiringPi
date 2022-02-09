#include <cursesw.h>
#include <iostream>
#include <ncursesw/curses.h>
#include <locale.h>
#include <string>
#include <array>

#include "mux.hpp"

// values of states
enum State {
  SAFE,
  ARMED,
  STARTUP,
  FIRING,
  SHUTDOWN,
  ABORT,
  ERROR,
  OFF,
};

const int num_states = OFF+1;

// names of states
std::array<const char*,num_states> state_names{
  "SAFE", 
  "ARMED",
  "STARTUP",
  "FIRING",
  "SHUTDOWN",
  "ABORT",
  "ERROR", // make sure errors are safely handled by abort during firing sequence
  "OFF"
};

// permissible states accessible from a given state
bool allowed_states [num_states][num_states] {
// SAFE   ARMED   STARTUP FIRING SHUTDOWN ABORT   ERROR   OFF
  {true,  true,   false,  false,  false,  false,  true,   true},  //SAFE
  {true,  true,   true,   false,  false,  false,  true,   false},  //ARMED
  {false, false,  true,   true,   true,   true,   false,  false}, //STARTUP
  {false, false,  false,  true,   true,   true,   false,  false}, //FIRING
  {true,  false,  false,  false,  true,   true,   true,   false}, //SHUTDOWN
  {false, false,  false,  false,  false,  true,   true,   false}, //ABORT
  {true,  false,  false,  false,  false,  false,  true,   true},  //ERROR
  {true,  false,  false,  false,  false,  false,  false,  true}   //OFF
};  

bool SOLENOID[3]{0};

void reinitwin(WINDOW * win, int height, int width, int starty,int startx)
{
  wmove(win, starty, startx);
  wresize(win, height, width);
  wclear(win);
  box(win,0,0);
  wrefresh(win);
}

void draw_state(WINDOW * win, State state)
{
  attron(A_REVERSE);
  mvprintw(3,5,"                ");
  mvprintw(4,5," CURRENT STATE  ");
  mvprintw(5,5,"                ");
  attroff(A_REVERSE);
  for (int i=0; auto state_name: state_names) 
  {
    if (state==i) { attron(A_REVERSE); }  
    else if (!allowed_states[state][i]) { attron(A_DIM); }
    mvprintw(2*i+7, 7, state_name);
    i++;
    attroff(A_REVERSE|A_DIM);
  };
}

int main()
{
  // systems stuff
  State state = OFF;
  State requested = OFF;

  // ncurses stuff
  setlocale(LC_ALL, "");
  WINDOW *main_win, *state_win, *valves_win;
  int ch;

  initscr();
  start_color();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  cbreak();
  refresh();

  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  main_win = newwin(0,0,1,1);
  state_win = newwin(0,0,2,24);
  valves_win = newwin(0,0,2,3);

  ungetch(KEY_RESIZE);
  
  while((ch = getch()) != 'q')
  {
    switch (ch)
    {
      case KEY_RESIZE:
        wclear(stdscr);
        refresh();
        //        window      height    width     y x 
        reinitwin(main_win,   LINES-2,  COLS-2,   1,1);
        reinitwin(state_win,  10,       COLS-27,  3,24);
        reinitwin(valves_win, LINES-4,  20,       3,3);
        break;
      case 's':
        requested = SAFE; break;
      case 'a'-96:
        requested = ARMED; break;
      case KEY_BACKSPACE:
        requested = ABORT; break;
      case 'o':
        requested = OFF;


       
    }
    if (allowed_states[state][requested]) { state = requested; }

    draw_state(state_win,state);
    mvprintw(0, 0, "    ");
    mvaddch(0,0,'0'+state);
    mvaddch(0,2,ch);

  }
  endwin();
  return 0;
}

