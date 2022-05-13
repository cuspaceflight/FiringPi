#include <string>
#include <hx711/common.h>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include "LoadCell.hpp"

int main() {
    std::vector<PT*> pts{
        new PT(1,PT_ADDR,SAMPLING_FREQ),
        new PT(4,PT_ADDR,SAMPLING_FREQ)
    };

    Relay relays;
    std::shared_ptr<LoadCell> load_cell = std::make_shared<LoadCell>(5, 6);
    StateMachine machine{&relays};
    Logger logger{&machine, &relays, &pts};
    Display display{&machine, &relays, &pts, load_cell, &logger};


    while (display.open) {
        display.update();
        machine.process();
    }
}
