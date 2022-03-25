#include <cursesw.h>
#include <iostream>

#include <ncursesw/curses.h>

#include <clocale>
#include <chrono>
#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"

int main() {
    Relay relays;
    StateMachine machine{&relays};
    Display display{&machine, &relays};
    PT pt{0, 0x28};

    std::cout << pt.measure();

    while (display.open) {
        display.update();
        machine.process();
    }
}
