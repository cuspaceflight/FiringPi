#pragma once

#include <thread>
#include <hx711/SimpleHX711.h>
#include <chrono>
#include <vector>
#include <iostream>

class LoadCell
{
public:
    LoadCell(int data, int clock);

    float get_weight() const;
    bool init();
    void kill();

    float weight;
private:
    // Data and clock pins
    const int DT,CLK;
    float k_filter;
    std::unique_ptr<HX711::SimpleHX711> load_cell;
    std::unique_ptr<std::thread> obj;
    HX711::Options options;

    void loop();
    bool is_alive = false;

};
