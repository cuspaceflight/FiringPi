#include "State.hpp"
#include <chrono>
#include <thread>

// names of states
const char* StateMachine::names[] { 
  "SAFE", "ARMED", "STARTUP", "FIRING", "SHUTDOWN", "ABORT", "ERROR", "OFF" 
};

// ncurses colors of states
const int StateMachine::colors[] {
  10, 11, 12, 12, 12, 9, 9, 0 
};

// permissible states accessible from a given state
const bool StateMachine::transition_matrix [NUM_STATES][NUM_STATES] {
// SAFE   ARMED   STARTUP FIRING SHUTDOWN ABORT   ERROR   OFF
  {true,  true,   false,  false,  false,  false,  true,   true},  //SAFE
  {true,  true,   true,   false,  false,  false,  true,   false},  //ARMED
  {false, false,  true,   true,   false,  true,   false,  false}, //STARTUP
  {false, false,  false,  true,   true,   true,   false,  false}, //FIRING
  {true,  false,  false,  false,  true,   true,   true,   false}, //SHUTDOWN
  {false, false,  false,  false,  false,  true,   true,   false}, //ABORT
  {true,  false,  false,  false,  false,  false,  true,   true},  //ERROR
  {true,  false,  false,  false,  false,  false,  false,  true}   //OFF
};  

StateMachine::StateMachine()
{
  state = OFF;
}

bool StateMachine::canChangeTo(State next)
{
  if (transition_matrix[state][next])
  {
    return true;
  } else {
    return false;
  }
}

bool StateMachine::changeState(State next)
{
  if (transition_matrix[state][next]) {
    state = next;
    state_begin = std::chrono::system_clock::now();
    return true;
  }
  else {
    return false;
  }
}

State StateMachine::update(int ch)
{
  switch(ch)
  {
    case 's':
      changeState(SAFE); break;
    case 'a'-96:
      changeState(ARMED); break;
    case ' ':
      changeState(SHUTDOWN);
      changeState(FIRING);
      changeState(STARTUP); break;
    case 263:
      changeState(ABORT); break;
    case 'e':
     changeState(ERROR); break;
    case 'o':
      changeState(OFF); break;
  }
  return state;
}

void StateMachine::process()
{
  switch (state)
  {
    case STARTUP:
    case FIRING:
    case SHUTDOWN:
      break;
    default:
      break;
  }
}
