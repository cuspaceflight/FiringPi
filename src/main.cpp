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
// std::array<const char*,num_states> state_names{
const char *state_names[num_states] {
  "SAFE", 
  "ARMED",
  "STARTUP",
  "FIRING",
  "SHUTDOWN",
  "ABORT",
  "ERROR", // make sure errors are safely handled by abort during firing sequence
  "OFF"
};

const int state_colors[num_states] {
  10,
  11,
  12,
  12,
  12,
  9,
  9,
  0
};

// permissible states accessible from a given state
bool state_transition_matrix [num_states][num_states] {
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

void draw_state(WINDOW *win, State state)
{
  attron(COLOR_PAIR(state_colors[state])|A_REVERSE);
  mvprintw(3,5,"                ");
  mvprintw(4,5," CURRENT STATE  ");
  mvprintw(5,5,"                ");
  attroff(COLOR_PAIR(state_colors[state])|A_REVERSE);
  for (int i=0; auto state_name: state_names) 
  {
    if (state==i) { attron(A_REVERSE); }  
    else if (!state_transition_matrix[state][i]) { attron(A_DIM); }
    mvprintw(2*i+7, 7, state_name);
    i++;
    attroff(A_REVERSE|A_DIM);
  };
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
  State state = OFF;
  State requested = OFF;

  // ncurses stuff
  setlocale(LC_ALL, "");
  WINDOW *main_win, *state_win, *valves_win;

  initscr();
  start_color();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  cbreak();
  refresh();

  if (has_colors() && can_change_color())
  {
    for (NCURSES_COLOR_T i; i<COLORS; i++)
    {
      init_pair(i, i, COLOR_BLACK);
    }
  }

  main_win = newwin(0,0,1,1);
  state_win = newwin(0,0,2,24);
  valves_win = newwin(0,0,2,3);

  ungetch(KEY_RESIZE);

  bool exit {};
  
  while(!exit)
  {
    switch (getch())
    {
      case 'q':
        if (state==OFF) { exit = true; }
        mvprintw(0,6,"MUST BE POWERED OFF TO EXIT: ENTER SAFE STATE THEN PRESS 'o'"); break;
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
      // TODO FOR DEBUG
      case ' ': 
        requested = (State)(state + 1); break;
      case KEY_BACKSPACE:
        requested = ABORT; break;
      case 'o':
        requested = OFF;
    }
    if (state_transition_matrix[state][requested] & requested<num_states) { state = requested; }
    draw_state(state_win,state);
  }
  endwin();  
  return 0;
}

