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

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"

int main()
{
  Relay relays;
  StateMachine machine{&relays};
  Display display{&machine,&relays};

  bool exit {0};
  
  while(!exit)
  {
    display.update();
    machine.process();
  }
  return 0;
}
