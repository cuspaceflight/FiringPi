#include <cursesw.h>
#include <iostream>
#include <ncursesw/curses.h>
#include <locale.h>
#include <chrono>
#include <string>

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
  NUM_STATES
};

// names of states
const char *state_names[NUM_STATES] {
  "SAFE", 
  "ARMED",
  "STARTUP",
  "FIRING",
  "SHUTDOWN",
  "ABORT",
  "ERROR", // make sure errors are safely handled by abort during firing sequence
  "OFF"
};

const int state_colors[NUM_STATES] {
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
bool state_transition_matrix [NUM_STATES][NUM_STATES] {
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
  mvwin(win, starty, startx);
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
  State state = OFF;
  State requested = OFF;
  
  init_ncurses();
  // ncurses stuff
  WINDOW *main_win, *state_win, *valves_win;
  

  main_win = newwin(0,0,1,1);
  state_win = newwin(0,0,2,24);
  valves_win = newwin(0,0,2,3);

  ungetch(KEY_RESIZE);

  bool exit {};
  auto state_alarm = std::chrono::system_clock::now(); // TODO have different alarms to avoid clashes
  
  const char* hint = "PRESS s[afe] OR q[uit]";
  cchar_t space {};
  setcchar(&space, L" ", 0, 0, (void*)0);
  
  while(!exit)
  {
    switch (getch())
    {
      case 'q':
        if (state==OFF) { exit = true; }
        hint = "MUST BE POWERED OFF TO EXIT: ENTER s[afe] STATE THEN PRESS o[ff], q[uit]"; break;
      case 'r':
        // refresh terminal settings
      case KEY_RESIZE:
        wclear(stdscr);
        refresh();
        //        window      height    width     y x 
        reinitwin(main_win,   LINES-2,  COLS-2,   1,1);
        reinitwin(state_win,  10,       COLS-27,  2,24);
        reinitwin(valves_win, LINES-4,  20,       2,3);
        break;
      case 's':
        requested = SAFE; break;
      case 'a'-96:
        requested = ARMED; break;
      // TODO FOR DEBUG
      case ' ': 
        requested = STARTUP; 
        state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1000); 
        break;
      case KEY_BACKSPACE:
        requested = ABORT;
        state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1500); 
        break;
      case 'o':
        requested = OFF;
    }

    if (std::chrono::system_clock::now() > state_alarm) {
      switch (state) 
      {
        case STARTUP:
          requested=FIRING;
          state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(3000); 
          break;
        case FIRING:
          requested=SHUTDOWN;
          state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1000); 
          break;
        case SHUTDOWN:
          requested=SAFE;
          break;
        case ABORT:
          requested=ERROR;
          break;
        default:
          break;
      }
    }
     
    if (state_transition_matrix[state][requested] && ((int)requested<NUM_STATES) && state != requested) {
      state = requested;
      hint="";
    }
    
    draw_state(state_win,state);
    // draw_colors();
    
    mvhline_set(0, 0, &space, COLS); 

    attron(COLOR_PAIR(3));
    mvprintw(0, 2, hint);
    attroff(A_COLOR);
  }
  endwin();  
  return 0;
}


