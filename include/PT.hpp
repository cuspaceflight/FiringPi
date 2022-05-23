#pragma once

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>

#define PMAX 350
#define PMIN 0


class PT {
public:

    float pressure, temperature;
    int file, addr, freq, status;

    PT(int bus, int address, int frequency);

    int recv();
    void loop();

    std::thread* thread_obj;
    bool is_alive;

};