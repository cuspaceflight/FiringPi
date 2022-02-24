#include "Relay.hpp"
#include <cstdlib>

Relay::Relay()
{
  if (!wiringPiSetup())
  {
    exit(EXIT_FAILURE);
  }
}

