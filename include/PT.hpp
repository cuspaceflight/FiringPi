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

#define PMAX 350
#define PMIN 0


class PT {

    float _pressure, _temperature;

public:
    PT(int bus, int addr);

    int recv();

    int file, addr, frequency;

    decltype(_pressure) pressure() const { return _pressure; }
    decltype(_temperature) temperature() const { return _temperature; }

};