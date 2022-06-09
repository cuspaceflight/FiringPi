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
    ARMED,
    STARTUP,
    FIRING,
    SHUTDOWN,
    ABORT,
    ERROR,
    OFF,
    NUM_STATES
};

class StateMachine {
public:
    State state;
    static const char *names[NUM_STATES];
    static const int colors[NUM_STATES];

    StateMachine(std::shared_ptr<Relay> valves, std::shared_ptr<Servo> servos);

    void changeState(State next);

    bool canChangeTo(State next) const;

    State update(int ch);

    void process() const;

private:
    std::shared_ptr<Relay> valves;
    std::shared_ptr<Servo> servos;
    std::chrono::time_point<std::chrono::system_clock> state_begin;
    static const bool transition_matrix[NUM_STATES][NUM_STATES];

    const std::vector<std::array<int, NUM_VALVES>> *valve_matrix;

    static const std::vector<std::array<int, NUM_VALVES>> IPA_cft_solenoids;
    static const std::vector<std::array<int, NUM_VALVES>> N2O_cft_solenoids;
    static const std::vector<std::array<int, NUM_VALVES>> H2O_cft_solenoids;

};
