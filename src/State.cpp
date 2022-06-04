#include "State.hpp"

#include <utility>

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

        {true,  true,  false, false, false, false, false, true,  true},  //INIT
        {false, true,  true,  false, false, false, false, true,  true},  //SAFE
        {false, true,  true,  true,  false, false, false, true,  false}, //ARMED
        {false, false, false, true,  true,  false, true,  false, false}, //STARTUP
        {false, false, false, false, true,  true,  true,  false, false}, //FIRING
        {false, true,  false, false, false, true,  true,  true,  false}, //SHUTDOWN
        {false, false, false, false, false, false, true,  true,  false}, //ABORT
        {false, true,  false, false, false, false, false, true,  true},  //ERROR
        {false, true,  false, false, false, false, false, false, true}   //OFF
};

const std::vector<std::array<int, NUM_VALVES>> StateMachine::IPA_cft_solenoids{
        {{0, 0, 0, 0, 0, 0, 1},
         {1, 0, 0, 0, 0, 0, 0},
         {1, 1, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0, 0},
         {1, 0, 1, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 1, 1, 1}}
};

const std::vector<std::array<int, NUM_VALVES>> StateMachine::N2O_cft_solenoids{
        {{0, 0, 0, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 1, 0},
         {6, 6, 6, 6, 6, 6, 6},
         {0, 0, 0, 0, 0, 0, 0},
         {1, 0, 1, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0, 0},
         {1, 1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 1, 1, 1}}
};

const std::vector<std::array<int, NUM_VALVES>> StateMachine::H2O_cft_solenoids{
        {{0, 0, 0, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0},
         {1, 0, 0, 0, 0, 0, 0},
         {1, 1, 1, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 1, 1, 1}}
};

StateMachine::StateMachine(std::shared_ptr<Relay> valves) : state(INIT), valves(valves), valve_matrix(&N2O_cft_solenoids) {}

bool StateMachine::canChangeTo(State next) const {
    return transition_matrix[state][next];
}

void StateMachine::changeState(State next) {
    state = transition_matrix[state][next] ? next : state;
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
    valves->set_outputs(&(valve_matrix->at(state)));
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
