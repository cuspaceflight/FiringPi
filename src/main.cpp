#include <string>
#include <hx711/common.h>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "LoadCell.hpp"

int main() {
    std::vector<PT*> pts{
        new PT(1,0x28,200),
        new PT(4,0x28,200)
    };

    Relay relays;
    std::shared_ptr<LoadCell> load_cell = std::make_shared<LoadCell>(5, 6);
    StateMachine machine{&relays};
    Display display{&machine, &relays, &pts, load_cell};


    while (display.open) {
        display.update();
        machine.process();
    }
}
