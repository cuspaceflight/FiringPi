#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include "LoadCell.hpp"
#include "ADC.hpp"
// #include "Servo.hpp"

int main() {
    auto PTs = std::make_shared < std::vector < PT * >> ();

    bool i2cHolds[7] = { false };
    PTs->push_back(new PT(1, M32JM_ADDR, SAMPLING_FREQ, &i2cHolds[1])); // PT0: Fuel tank
    PTs->push_back(new PT(3, M32JM_ADDR, SAMPLING_FREQ, &i2cHolds[3])); // PT1: Ox tank
    PTs->push_back(new PT(4, M32JM_ADDR, SAMPLING_FREQ, &i2cHolds[4])); // PT2: Fuel Manifold
    PTs->push_back(new PT(5, M32JM_ADDR, SAMPLING_FREQ, &i2cHolds[5])); // PT3: Ox Manifold
    PTs->push_back(new PT(6, M32JM_ADDR, SAMPLING_FREQ, &i2cHolds[6])); // PT4: Chamber

    auto LCs = std::make_shared < std::vector < LoadCell * >> ();

    LCs->push_back(new LoadCell(7, 25, LC0_REF_UNIT, LC0_OFFSET));   // Fuel tank
    //LCs->push_back(new LoadCell(24, 18, LC1_REF_UNIT, LC1_OFFSET)); // Ox tank TODO remove
    //LCs->push_back(new LoadCell(15, 14, LC2_REF_UNIT, LC2_OFFSET)); // Thrust TODO remove

    auto ADCs = std::make_shared < std::vector < ADC * >> ();
    ADCs->push_back(new ADC(1, ADC0_ADDR, 200, &i2cHolds[1])); // ADC0: Thermocouples and LP water PTs
    //ADCs->push_back(new ADC(3, ADC0_ADDR, 200)); // ADC1: N/C

    auto relays = std::make_shared<Relay>();
//    auto servos = std::make_shared<Servo>(6, 40); // Servo 1: He Valve
    auto machine = std::make_shared<StateMachine>(relays);
    auto logger = std::make_shared<Logger>(machine, relays, PTs, LCs);
    auto display = std::make_shared<Display>(machine, relays, PTs, LCs, ADCs, logger);

    while (display->open) {
        display->update();
        machine->process();
    }
}
