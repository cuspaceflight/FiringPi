#pragma once

#include <fstream>
#include <thread>
#include <chrono>

#include "State.hpp"
#include "Relay.hpp"
#include "PT.hpp"


#define LOGGING_FREQ 200

class Logger {

private:
    ofstream logfile;
    StateMachine *machine;
    Relay *relays;
    std::vector<PT*> *pts;
    std::chrono::time_point<std::chrono::system_clock> now, last;

public:
    PT(StateMachine *machine, Relay *relays, std::vector<PT *> *pts)
    this->machine = machine;
    this->relays = relays;
    this->pts = pts;
}
