#include "Relay.hpp"

const int Relay::pins[N_CHANNELS] = {18, 26, 6, 11, 10, 27, 17, 18};

const char* Relay::channel_names[N_CHANNELS] = {
        "---        CH0", // HE MAIN
        "N2O FILL   CH1",
        "N2O VENT   CH2",
        "IPA VENT   CH3",
        "IGNITER-1  CH4",
        "IGNITER-2  CH5",
        "IGNITER-3  CH6",
        "---        CH7", // CHAM VENT
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
        if (i==1) { // this valve is normally open
            digitalWrite(pins[i], values[i]);
        } else {
            digitalWrite(pins[i], 1 - values[i]);
        }
    }
}
