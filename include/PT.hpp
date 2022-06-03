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

#define PMAX 350
#define PMIN 0

enum PT_MODEL {
    M32JM_00010B_350BG,
    SEEED_114991178,
    PXM319_070GI
};



class PT {
public:

    float pressure, temperature;
    int file, addr, freq;
    PT_MODEL model;

    PT(int bus, int address, int frequency, PT_MODEL model);

    int recv();
    void loop();

    std::thread* thread_obj;
    bool is_alive;

};