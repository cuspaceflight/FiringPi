#include "Relay.hpp"

const int Relay::pins[N_CHANNELS] = {26, 19, 11, 10, 27, 17, 6, 16};

const char* Relay::channel_names[N_CHANNELS] = {
        "---        CH0", // HE MAIN
        "N2O FILL   CH1",
        "---        CH2", // CHAM VENT
        "N2O VENT   CH3",
        "IPA VENT   CH4",
        "IGNITER-1  CH5",
        "IGNITER-2  CH6",
        "IGNITER-3  CH7"
};

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

void Relay::set_outputs(const int values[N_CHANNELS]) {
    for (int i = 0; i < N_CHANNELS; i++) {
        digitalWrite(pins[i], 1 - values[i]);
    }
}
