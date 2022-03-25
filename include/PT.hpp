#pragma once

#include <linux/i2c-dev.h>
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <sys/ioctl.h>


class PT {
public:
    PT(int bus, int addr);

    int measure();

    int read(float p, float t);

private:
    int file, addr;
};