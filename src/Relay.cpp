#include "Relay.hpp"

const int Relay::pins[NUM_VALVES] = {26, 19, 11, 10, 27};

Relay::Relay() {
    // no need to check error code of wiringPiSetup()
    // as it always returns zero and will crash on failure
    wiringPiSetupGpio();
    for (int pin: pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, 0);
    }
}

int Relay::get_output(int output) {return  (int)(1-digitalRead(pins[output])); }

void Relay::set_output(int output, int value) { digitalWrite(pins[output],1-value); }

void Relay::set_outputs(const int values[NUM_VALVES]) {
    for (int i = 1; i < NUM_VALVES; i++) {    // use i=1 because i=0 represents the helium valve servo
        digitalWrite(pins[i - 1], 1 - values[i]);
    }
}
