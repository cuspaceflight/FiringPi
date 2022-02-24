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
#include "Relay.hpp"


bool SOLENOID[3]{0};


int main()
{
  StateMachine machine;
  Display display{&machine};
  Relay relays;

  bool exit {0};
  
  while(!exit)
  {
    display.update();
    machine.process();
  }
  return 0;
}
