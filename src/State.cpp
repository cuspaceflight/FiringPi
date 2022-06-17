#include "State.hpp"

// names of states
const char *StateMachine::names[]{
        "INIT", "[S]AFE", "[^R]EADY", "[F]UEL FILL", "[O]X FILL", "C[H]ILL",
        "[C]HAMBER PRESS", "[T]ANKS PRESS", "[I]GNITION", "[_]FIRING", "[x]SHUTDOWN",
        "[V]ENT", "[<-]ABORT", "[E]RROR", "[^O]FF", "[^M]ANUAL"
};

// ncurses colors of states
const int StateMachine::colors[]{
        10, 10, 11, 12, 12, 12,
        12, 12, 208, 208, 208,
        220, 9, 9, 0, 199
};

// permissible states accessible from a given state
const bool StateMachine::transition_matrix[NUM_STATES][NUM_STATES]{
//    INIT SAFE READY IGNITION FIRING SHUTDOWN ABORT ERROR OFF

//        {true,  true,  false, false, false, false, false, true,  true},  //INIT
//        {false, true,  true,  false, false, false, false, true,  true},  //SAFE
//        {false, true,  true,  true,  false, false, false, true,  false}, //READY
//        {false,false,false,false,false,false,false,false,false,false,false,false,},
//        {},
//        {},
//        {},
//        {},
//        {false, false, false, true,  true,  false, true,  false, false}, //IGNITION
//        {false, false, false, false, true,  true,  true,  false, false}, //FIRING
//        {false, true,  false, false, false, true,  true,  true,  false}, //SHUTDOWN
//        {},
//        {false, false, false, false, false, false, true,  true,  false}, //ABORT
//        {false, true,  false, false, false, false, false, true,  true},  //ERROR
//        {false, true,  false, false, false, false, false, false, true}   //OFF
        // ALL TRUE FOR DEBUG
        //    INIT      SAFE             READY   FUEL FILL   OX FILL      CHILL       CHAM P    TANKS P     IGNITION    FIRING    SHUTDOWN    VENT        ABORT       ERROR       OFF   MANUAL
        {true,  true,  false, false, false, false, false, false, false, false, false, false, false, true,  true,  true}, // INIT
        {false, true,  true,  false, false, false, false, false, false, false, false, false, false, true,  true,  true}, // SAFE
        {false, true,  true,  true,  true,  true,  true,  true,  true,  false, false, true,  false, true,  false, true}, // READY
        {false, false, true,  true,  false, false, false, false, false, false, false, false, false, true,  false, true}, // F FILL
        {false, false, true,  false, true,  false, false, false, false, false, false, false, false, true,  false, true}, // O FILL
        {false, false, true,  false, false, true,  false, false, false, false, false, false, false, true,  false, true}, // CHILL
        {false, false, true,  false, false, false, true,  false, false, false, false, false, false, true,  false, true}, // CHAM P
        {false, false, true,  false, false, false, false, true,  false, false, false, false, false, true,  false, true}, // TANK P
        {false, false, false, false, false, false, false, false, true,  true,  false, false, true,  false, false, true}, // IGNITION
        {false, false, false, false, false, false, false, false, false, true,  true,  false, true,  false, false, true}, // FIRING
        {false, false, true,  false, false, false, false, false, false, false, true,  false, false, true,  false, true}, // SHUTDOWN
        {false, false, true,  false, false, false, false, false, false, false, false, true,  false, true,  false, true}, // VENT
        {false, false, true,  false, false, false, false, false, false, false, false, false, true,  true,  false, true}, // ABORT
        {false, false, true,  false, false, false, false, false, false, false, false, false, false, true,  true,  true}, // ERROR
        {false, true,  false, false, false, false, false, false, false, false, false, false, false, false, true,  true}, // OFF
        {true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true} // OFF

};

const int StateMachine::valve_matrix[NUM_STATES][NUM_VALVES]{
        // HE MAIN(SERVO), N2O FILL(NO), CHAM VENT, N2O VENT, IPA VENT, IGNITER -- NOTE, ASSUMED ALL VALVES ARE NC, 1 = OPEN, 0 = CLOSED
        {1, 1, 1, 1, 1, 0}, // INIT
        {0, 0, 1, 1, 1, 0}, // SAFE
        {0, 0, 0, 0, 0, 0}, // READY
        {0, 0, 0, 0, 1, 0}, // FUEL FILL
        {0, 1, 0, 0, 0, 0}, // OX FILL
        {0, 0, 0, 1, 0, 0}, // CHILL
        {1, 0, 0, 0, 0, 0}, // PRESS CHAMBER
        {1, 0, 0, 0, 0, 0}, // PRESS TANKS
        {1, 0, 0, 0, 0, 1}, // IGNITION
        {1, 0, 0, 0, 0, 0}, // FIRING
        {0, 0, 0, 0, 0, 0}, // SHUTDOWN
        {0, 0, 1, 1, 1, 0}, // VENT
        {0, 0, 1, 0, 0, 0}, // ABORT (VENT CHAMBER)
        {0, 0, 0, 0, 0, 0}, // ERROR
        {0, 0, 0, 0, 0, 0} // OFF
};

StateMachine::StateMachine(std::shared_ptr <Relay> valves, std::shared_ptr <Servo> servos)
        : state(INIT), valves(valves), servos(servos) {}

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
        case 'r':
            if (state != SAFE) changeState(READY);
            break;
        case 'r' - 96:
            changeState(READY);
            break;
        case 'f':
            changeState(F_FILL);
            break;
        case 'o':
            changeState(O_FILL);
            break;
        case 'h':
            changeState(CHILL);
            break;
        case 'c':
            changeState(P_CHAM);
            break;
        case 't':
            changeState(P_TANKS);
            break;
        case 'i':
            changeState(IGNITION);
            break;
        case ' ':
            changeState(FIRING);
            break;
        case 'x':
            changeState(SHUTDOWN);
            break;
        case 'v':
            changeState(VENT);
            break;
        case 263:
            changeState(ABORT);
            break;
        case 'e':
            changeState(ERROR);
            break;
        case 'o' - 96:
            changeState(OFF);
            break;
        case 'm' - 96:
            changeState(MANUAL);
            break;
        case '0':
            if (state == MANUAL) servos->write_position(0, SERVO_OPEN - this->position);
            break;
        case '1','2', '3', '4', '5', '6':
            if (state == MANUAL) valves->set_output(ch - '0', 1 - valves->get_ouput(ch - '0'));
            break;
        default:
            break;
    }
    return state;
}

void StateMachine::process() const {
    switch (state) {
        case INIT:
            for (int i = 0; auto *LC: *LCs) i += (int) LC->init();
            if (i == LCs->size()) changeState(SAFE);
            else display->write_error("Check load cells");
            valves->set_outputs(valve_matrix[state]);
            servos->write_position(0, valve_matrix[state][0] * SERVO_OPEN);
            break;
        case MANUAL:
            break;
        case
        default:
            valves->set_outputs(valve_matrix[state]);
            servos->write_position(0, valve_matrix[state][0] * SERVO_OPEN);
            break
    }

}
