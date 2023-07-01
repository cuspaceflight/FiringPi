#pragma once

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <fcntl.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include "../include/ADS1115.hpp"

#define PMAX 350
#define PMIN 0


class PT {
public:

    float pressure, temperature, k_filter{0.4};
    int file, addr, freq;
    ADS1115 adc;

    PT(int bus, int address, int frequency);

    bool setAddr(void);
    int recv();
    void loop();

    std::thread* thread_obj;
    bool is_alive;

};