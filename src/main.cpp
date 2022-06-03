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
        new PT(0,PT_ADDR,SAMPLING_FREQ,M32JM_00010B_350BG),
        new PT(3,PT_ADDR,SAMPLING_FREQ,M32JM_00010B_350BG),
    };

    Relay relays;
    std::shared_ptr<LoadCell> load_cell = std::make_shared<LoadCell>(5, 6);
    StateMachine machine{&relays};
    Logger logger{&machine, &relays, &pts};
    Display display{&machine, &relays, &pts, load_cell, &logger};

    if (load_cell->init()) {
        machine.update('o');
    } else {
        machine.update('e');
        display.write_error("Check load cell");
    }

    while (display.open) {
        display.update();
        machine.process();
    }
}
