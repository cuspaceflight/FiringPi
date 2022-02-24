#pragma once

#include <wiringPi.h>
#include <stdlib.h>

#define NUM_RELAYS 8


class Relay
{
public:
  Relay();
  void set_output(int output, bool value);
private:
  int outputs[NUM_RELAYS];
};
