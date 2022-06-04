#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include "LoadCell.hpp"

int main() {
    auto pts = std::make_shared<std::vector<PT*>>();

    pts->push_back(new PT(0, PT_ADDR, SAMPLING_FREQ, M32JM_00010B_350BG));
    pts->push_back(new PT(3, PT_ADDR, SAMPLING_FREQ, M32JM_00010B_350BG));


    auto relays = std::make_shared<Relay>();
    auto load_cell = std::make_shared<LoadCell>(5, 6);
    auto machine = std::make_shared<StateMachine>(relays);
    auto logger = std::make_shared<Logger>(machine, relays, pts);
    auto display = std::make_shared<Display>(machine, relays, pts, load_cell, logger);

    if (load_cell->init()) {
        machine->update('o');
    } else {
        machine->update('e');
        display->write_error("Check load cell");
    }

    while (display->open) {
        display->update();
        machine->process();
    }
}
