#include <cursesw.h>
#include <iostream>

#include <ncursesw/curses.h>

#include <clocale>
#include <chrono>
#include <string>
#include <bitset>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"

int main() {
    PT pt{1, 0x28};
//
//    float p,t;
//    pt.recv(&p,&t);
//    printf("P: %f\nT: %f\n",p,t);



    Relay relays;
    StateMachine machine{&relays};
    Display display{&machine, &relays, &pt};


    while (display.open) {
        display.update();
        machine.process();
    }
}
