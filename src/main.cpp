#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"

#define SAMPLING_FREQ 200   // Sammpling frequency, Hz
#define PT1_ADDR      0x28       // PT1 Address

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
        logger.log();
    }
}
