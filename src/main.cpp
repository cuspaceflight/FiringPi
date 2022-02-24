#include <cursesw.h>
#include <iostream>
#if __has_include(<ncursesw/curses.h>)
    #include <ncursesw/curses.h>
#else
    #include <ncurses.h>
#endif
#include <locale.h>
#include <chrono>
#include <string>
#include <thread>

#include "State.hpp"
#include "Display.hpp"


bool SOLENOID[3]{0};


int main()
{
  StateMachine machine {};
  Display display {};

  bool exit {0};
  
  while(!exit)
  {
    display.update(machine);
    machine.process();
  }
  return 0;
}
