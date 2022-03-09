#pragma once

#include <chrono>

#include "Relay.hpp"

// values of states
enum State {
  SAFE,
  ARMED,
  STARTUP,
  FIRING,
  SHUTDOWN,
  ABORT,
  ERROR,
  OFF,
  NUM_STATES
};

class StateMachine
{
public:
  State state;
  static const char* names[NUM_STATES];
  static const int colors[NUM_STATES];

  StateMachine(Relay* valves);
  bool changeState(State next);
  bool canChangeTo(State next);
  State update(int ch);
  void process();
private:
  Relay* valves;
  std::chrono::time_point<std::chrono::system_clock> state_begin;  
  static const bool transition_matrix[NUM_STATES][NUM_STATES];
  static const int valve_matrix[NUM_STATES][NUM_RELAYS];

};
