#pragma once

#include <wiringPi.h>
#include <iostream>
#include <stdexcept>
#include <array>

#define NUM_VALVES 7

class Relay {
public:
    Relay();

    static void set_output(int output, int value);

    static void set_outputs(const std::array<int, NUM_VALVES> *values);

private:
    static const int pins[NUM_VALVES];
};
