#include "PT.hpp"

PT::PT(int bus, int addr) {
    this->addr = addr;

    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    file = open(filename, O_RDWR);

    if (file < 0) {
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, this->addr) < 0) {
        exit(1);
    }

}

int PT::recv() {
    char buf[4];
    if (read(file, buf, 4) < 0) { return -1; }

    _pressure = (PMAX - PMIN) / 14000.0f * (float)(((buf[0] << 8) | buf[1]) - 1000) + PMIN;
    _temperature = (200 / 2048.0f) * (float)((buf[2] << 3) | (buf[3] >> 5)) - 50.0f;

    read(file, buf, 0);
    return 0;
}
