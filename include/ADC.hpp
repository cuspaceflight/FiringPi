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

enum Sensor {
    SEEED_114991178,
    PXM319_070GI,
    PT100
};

class ADC {
public:
    ADC(int bus, int addr, int freq);

    Sensor types[4];
    int file, freq;
    float values[4];

    int recv();
    void loop();

    std::thread* thread_obj;
    bool is_alive;

};