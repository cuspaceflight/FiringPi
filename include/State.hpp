#pragma once

#include <chrono>
#include <vector>
#include <array>
#include <memory>

#include "Relay.hpp"
#include "Servo.hpp"

// values of states
enum State {
    INIT,
    SAFE,
    READY,
    F_FILL,
    O_FILL,
    CHILL,
    P_CHAM,
    P_TANKS,
    IGNITION,
    FIRING,
    SHUTDOWN,
    VENT,
    ABORT,
    ERROR,
    OFF,
    MANUAL,
    NUM_STATES
};

class StateMachine {
public:
    State state;
    static const char *names[NUM_STATES];
    static const int colors[NUM_STATES];

    StateMachine(std::shared_ptr<Relay> valves, std::shared_ptr<Servo> servos);

    void changeState(State next);

    bool canChangeTo(State next);

    State update(int ch);

    void process();

private:
    std::shared_ptr<Relay> valves;
    std::shared_ptr<Servo> servos;
    std::chrono::time_point<std::chrono::system_clock> state_begin;
    static const bool transition_matrix[NUM_STATES][NUM_STATES];

    static const int valve_matrix[NUM_STATES][N_CHANNELS];
};
