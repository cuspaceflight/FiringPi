#include "../include/MAX31865.hpp"
#include <iostream>
#include <unistd.h>

int main() {
    MAX31865 rtd;

    if (!rtd.initialize()) {
        std::cerr << "Failed to initialize MAX31865 driver." << std::endl;
        return 1;
    }

    float temperature = rtd.readTemperature();
    std::cout << "Temperature: " << temperature << "°C" << std::endl;

    return 0;
}