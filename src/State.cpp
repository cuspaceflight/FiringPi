#include "State.hpp"

// names of states
const char *StateMachine::names[]{
        "INIT", "SAFE", "ARMED", "STARTUP", "FIRING", "SHUTDOWN", "ABORT", "ERROR", "OFF"
};

// ncurses colors of states
const int StateMachine::colors[]{
        10, 10, 11, 12, 12, 12, 9, 9, 0
};

// permissible states accessible from a given state
const bool StateMachine::transition_matrix[NUM_STATES][NUM_STATES]{
//    INIT SAFE ARMED STARTUP FIRING SHUTDOWN ABORT ERROR  OFF

        {true, true,  false,  false, false, false, false, true,  true},  //INIT
        {false, true,  true,  false, false, false, false, true,  true},  //SAFE
        {false, true,  true,  true,  false, false, false, true,  false}, //ARMED
        {false, false, false, true,  true,  false, true,  false, false}, //STARTUP
        {false, false, false, false, true,  true,  true,  false, false}, //FIRING
        {false, true,  false, false, false, true,  true,  true,  false}, //SHUTDOWN
        {false, false, false, false, false, false, true,  true,  false}, //ABORT
        {false, true,  false, false, false, false, false, true,  true},  //ERROR
        {false, true,  false, false, false, false, false, false, true}   //OFF
};

const int StateMachine::valve_matrix[NUM_STATES][NUM_RELAYS]{
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {0, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {1, 1, 1, 0},
        {0, 0, 0, 0}
};

StateMachine::StateMachine(Relay *valves) {
    state = INIT;
    this->valves = valves;
}

bool StateMachine::canChangeTo(State next) const {
    if (transition_matrix[state][next]) {
        return true;
    } else {
        return false;
    }
}

bool StateMachine::changeState(State next) {
    if (transition_matrix[state][next]) {
        state = next;
        return true;
    } else {
        return false;
    }
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
        case INIT:
            break;
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
