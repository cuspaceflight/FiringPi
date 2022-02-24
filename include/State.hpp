#pragma once

#include <chrono>

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
  std::chrono::time_point<std::chrono::system_clock> state_begin;  

  StateMachine();
  bool changeState(State next);
  bool canChangeTo(State next);
  State update(int ch);
  void process();

  static const char* names[NUM_STATES];
  static const int colors[NUM_STATES];
  static const bool transition_matrix[NUM_STATES][NUM_STATES];

};
