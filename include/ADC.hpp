#pragma once

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

#include <thread>
#include <iostream>
#include <unistd.h>
#include <zconf.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "../include/ADS1115.hpp"

enum Sensor {
    SEEED_114991178,
    PXM319_070GI,
    PT100
};

class ADC {
public:
    ADC(uint8_t bus, uint8_t address, int freq, bool* hold);

    uint8_t bus, address;
    int freq;
    float values[4], k_filter;
    bool is_alive;
    ADS1115 ads;
    bool* hold;

    int recv();
    void loop();

    std::thread* thread_obj;

};