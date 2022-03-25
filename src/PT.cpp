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
