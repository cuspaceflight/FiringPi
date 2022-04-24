#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"

int main() {
    std::vector<PT*> pts{
        new PT(1,0x28,200),
        new PT(4,0x28,200)
    };

    Relay relays;
    StateMachine machine{&relays};
    Display display{&machine, &relays, &pts};


    while (display.open) {
        display.update();
        machine.process();
    }
}
