#include <cursesw.h>
#include <iostream>
#include <ncursesw/curses.h>
#include <locale.h>
#include <chrono>
#include <string>

#include "Mux.hpp"
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
    else if (!machine.canChangeTo((StateIndex)i)) { attron(A_DIM); }
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
  auto state_alarm = std::chrono::system_clock::now(); // TODO have different alarms to avoid clashes
  
  const char* hint = "PRESS s[afe] OR q[uit]";
  cchar_t space {};
  setcchar(&space, L" ", 0, 0, (void*)0);
  
  while(!exit)
  {
    switch (getch())
    {
      case 'q':
        if (machine.state==OFF) { exit = true; }
        hint = "MUST BE POWERED OFF TO EXIT: ENTER s[afe] STATE THEN PRESS o[ff], q[uit]"; break;
      case KEY_RESIZE:
        wclear(stdscr);
        refresh();
        //        window      height    width     y x 
        reinitwin(main_win,   LINES-2,  COLS-2,   1,1);
        reinitwin(state_win,  10,       COLS-27,  2,24);
        reinitwin(valves_win, LINES-4,  20,       2,3);
        break;
      case 's':
        machine.changeState(SAFE); break;
      case 'a'-96:
        machine.changeState(ARMED); break;
      // TODO FOR DEBUG
      case ' ': 
        machine.changeState(STARTUP); 
        state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1000); 
        break;
      case KEY_BACKSPACE:
        machine.changeState(ABORT);
        state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1500); 
        break;
      case 'o':
        machine.changeState(OFF);
    }

    if (std::chrono::system_clock::now() > state_alarm) {
      switch (machine.state) 
      {
        case STARTUP:
          machine.changeState(FIRING);
          state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(3000); 
          break;
        case FIRING:
          machine.changeState(SHUTDOWN);
          state_alarm = std::chrono::system_clock::now() + std::chrono::milliseconds(1000); 
          break;
        case SHUTDOWN:
          machine.changeState(SAFE);
          break;
        case ABORT:
          machine.changeState(ERROR);
          break;
        default:
          break;
      }
    }
     
    draw_state(state_win,machine);
    draw_colors();
    
    mvhline_set(0, 0, &space, COLS); 

    attron(COLOR_PAIR(3));
    mvprintw(0, 2, hint);
    attroff(A_COLOR);
  }
  endwin();  
  return 0;
}


