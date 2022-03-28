#include "Relay.hpp"

const int Relay::pins[NUM_RELAYS] = {23, 24, 12, 25};

Relay::Relay() {
    // no need to check error code of wiringPiSetup()
    // as it always returns zero and will crash on failure
    wiringPiSetupGpio();
    for (int pin: pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, 0);
    }
}

void Relay::set_output(int output, int value) {
    digitalWrite(pins[output], value);
}

void Relay::set_outputs(int *values) {
    for (int i = 0; i < NUM_RELAYS; i++) {
        digitalWrite(pins[i], values[i]);
    }
}
