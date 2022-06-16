#include "LoadCell.hpp"

LoadCell::LoadCell(int data, int clock, HX71::Value refUnit, HX711::Value offset) :
DT(data), CLK(clock), k_filter(0.9) {
    load_cell = std::make_unique<HX711::SimpleHX711>(data, clock, refUnit, offset, HX711::Rate::HZ_80);
    //this->init();
}

bool LoadCell::init() {
    unsigned long long timeout = 1e9;
    std::vector<HX711::Value> init_vals = load_cell->getValues(std::chrono::nanoseconds(timeout));
    if (!init_vals.empty()) {
        is_alive = true;

        options.stratType=HX711::StrategyType::Samples;
        options.samples=1; // change this for actual operation perhaps

        obj = std::make_unique<std::thread>(&LoadCell::loop, this);
        return true;
    } else {
        return false;
    }
}

void LoadCell::loop() {
    while (is_alive) {
        // Handles things already
        weight *= (1 - k_filter);
        weight += (k_filter * (float)load_cell->read(options));
    }
}

void LoadCell::kill() {
    if (is_alive) {
        is_alive = false;
        obj->join();
    }
}

float LoadCell::get_weight() const {
    if (is_alive) {
        return weight;
    } else {
        return 0.0;
    }
}
