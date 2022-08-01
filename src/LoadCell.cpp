#include "LoadCell.hpp"

LoadCell::LoadCell(int data, int clock, HX711::Value refUnit, HX711::Value offset) :
DT(data), CLK(clock), k_filter(0.9), grad(-0.004123016321175486), offset(0.0) {
    load_cell = std::make_unique<HX711::SimpleHX711>(data, clock, refUnit, offset, HX711::Rate::HZ_80);
    //this->init(); TODO UNCOMMENT
}

bool LoadCell::init() {
    unsigned long long timeout = 1e9; // TODO DELETE
    std::size_t num_samples(240);
    std::vector<HX711::Value> init_vals = load_cell->getValues(num_samples);
    // offset=(1.0 * std::accumulate(init_vals.begin(), init_vals.end(), 0LL) / init_vals.size()) TODO UNCOMMENT
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
        return weight;                      // TODO DELETE
//        return (weight-offset)*grad;    TODO UNCOMMENT
    } else {
        return 0.0;
    }
}
