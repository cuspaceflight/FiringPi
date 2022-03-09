#pragma once

#include <wiringPi.h>
#include <iostream>
#include <stdexcept>

#define NUM_RELAYS 4

class Relay
{
public:
    Relay();
    static void set_output(int output, int value);
    static void set_outputs(int values[NUM_RELAYS]);
private:
    static const int pins[NUM_RELAYS];
};
