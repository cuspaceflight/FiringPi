#pragma once

// values of states
enum StateIndex {
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
  StateIndex state;
  StateMachine();
  bool changeState(StateIndex next);
  bool canChangeTo(StateIndex next);

  static const char* names[NUM_STATES];
  static const int colors[NUM_STATES];
  static const bool transition_matrix[NUM_STATES][NUM_STATES];

};
