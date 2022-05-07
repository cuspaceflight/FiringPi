#include "../include/LoadCell.hpp"

LoadCell::LoadCell(int data, int clock) : DT(data), CLK(clock)
{
    // TODO: Some error handling? idk

    load_cell = std::make_unique<HX711::SimpleHX711>(data, clock, 1, 0, HX711::Rate::HZ_80);
    obj = std::make_unique<std::thread>(&LoadCell::loop, this);
    is_alive = true;
}

void LoadCell::loop()
{
    while (is_alive)
    {
        // Handles things already
        weight = load_cell->getValues(1)[0];
    }
}

void LoadCell::kill()
{
    is_alive = false;
    obj->join();
}

float LoadCell::get_weight() const
{
    return weight;
}
