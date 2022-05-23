#include "State.hpp"

// names of states
const char *StateMachine::names[]{
        "SAFE", "ARMED", "STARTUP", "FIRING", "SHUTDOWN", "ABORT", "ERROR", "OFF"
};

// ncurses colors of states
const int StateMachine::colors[]{
        10, 11, 12, 12, 12, 9, 9, 0
};

// permissible states accessible from a given state
const bool StateMachine::transition_matrix[NUM_STATES][NUM_STATES]{
// SAFE   ARMED   STARTUP FIRING SHUTDOWN ABORT   ERROR   OFF
        {true,  true,  false, false, false, false, true,  true},  //SAFE
        {true,  true,  true,  false, false, false, true,  false}, //ARMED
        {false, false, true,  true,  false, true,  false, false}, //STARTUP
        {false, false, false, true,  true,  true,  false, false}, //FIRING
        {true,  false, false, false, true,  true,  true,  false}, //SHUTDOWN
        {false, false, false, false, false, true,  true,  false}, //ABORT
        {true,  false, false, false, false, false, true,  true},  //ERROR
        {true,  false, false, false, false, false, false, true}   //OFF
};

const int StateMachine::valve_matrix[NUM_STATES][NUM_RELAYS]{
        {1, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
};

StateMachine::StateMachine(Relay *valves) : state(OFF), valves(valves) {}

bool StateMachine::canChangeTo(State next) const {
    return transition_matrix[state][next];
}

bool StateMachine::changeState(State next) {
    state = transition_matrix[state][next]? next : state;
}

State StateMachine::update(int ch) {
    switch (ch) {
        case 's':
            changeState(SAFE);
            break;
        case 'a' - 96:
            changeState(ARMED);
            break;
        case ' ':
            changeState(SHUTDOWN);
            changeState(FIRING);
            changeState(STARTUP);
            break;
        case 263:
            changeState(ABORT);
            break;
        case 'e':
            changeState(ERROR);
            break;
        case 'o':
            changeState(OFF);
            break;
        default:
            break;
    }
    return state;
}

void StateMachine::process() const {
    Relay::set_outputs((int *) valve_matrix[state]);
    switch (state) {
        case SAFE:
            break;
        case ARMED:
            break;
        case STARTUP:
            break;
        case FIRING:
            break;
        case SHUTDOWN:
            break;
        case ABORT:
            break;
        case ERROR:
            break;
        case OFF:
            break;
        case NUM_STATES:
            break;
    }
}
