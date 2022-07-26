#pragma once

#include <wiringPi.h>
#include <iostream>
#include <stdexcept>
#include <array>

#define N_CHANNELS 8

class Relay {
public:
    Relay();

    static const char *channel_names[N_CHANNELS];

    int get_output(int output);

    static void set_output(int output, int value);

    static void set_outputs(const int values[N_CHANNELS]);

private:
    static const int pins[N_CHANNELS];
};
