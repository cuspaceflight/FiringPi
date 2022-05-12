#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include <fstream>

int main() {
    std::vector<PT*> pts{
        new PT(1,PT1_ADDR,SAMPLING_FREQ)
    };

    Relay relays;
    StateMachine machine{&relays};
    Logger logger{&machine, &relays, &pts};
    Display display{&machine, &relays, &pts, &logger};

    while (display.open) {
        display.update();
        machine.process();
    }
}
