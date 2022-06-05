#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include "LoadCell.hpp"
#include "ADC.hpp"

int main() {
    auto PTs = std::make_shared<std::vector<PT*>>();

    PTs->push_back(new PT(0, M32JM_ADDR, SAMPLING_FREQ));
    PTs->push_back(new PT(1, M32JM_ADDR, SAMPLING_FREQ));
    PTs->push_back(new PT(3, M32JM_ADDR, SAMPLING_FREQ));
    PTs->push_back(new PT(4, M32JM_ADDR, SAMPLING_FREQ));

    auto LCs = std::make_shared<std::vector<LoadCell*>>();

    LCs->push_back(new LoadCell(5,6));

    auto ADCs = std::make_shared<std::vector<ADC*>>();

    ADCs->push_back(new ADC(6,ADC0_ADDR,200));
    ADCs->push_back(new ADC(6,ADC1_ADDR,200));

    auto relays = std::make_shared<Relay>();
    auto machine = std::make_shared<StateMachine>(relays);
    auto logger = std::make_shared<Logger>(machine, relays, PTs);
    auto display = std::make_shared<Display>(machine, relays, PTs, LCs, ADCs, logger);

    if ((*LCs)[0]->init()) {
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
