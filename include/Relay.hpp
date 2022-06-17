#pragma once

#include <wiringPi.h>
#include <iostream>
#include <stdexcept>
#include <array>

#define NUM_VALVES 7

class Relay {
public:
    Relay();

    int get_output(int output);

    static void set_output(int output, int value);

    static void set_outputs(const int values[NUM_VALVES]);

private:
    static const int pins[NUM_VALVES];
};
