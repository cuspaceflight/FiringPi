#include "Logger.hpp"

Logger::Logger(StateMachine *machine, Relay *relays, std::vector<PT *> *pts) {
    this->machine = machine;
    this->relays = relays;
    this->pts = pts;
}