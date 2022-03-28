#include "PT.hpp"

PT::PT(int bus, int addr, int frequency) {
    this->addr = addr;
    this->frequency = frequency;
    this->is_alive = true;

    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    this->file = open(filename, O_RDWR);

    if (this->file < 0) {
        exit(1);
    }

    if (ioctl(this->file, I2C_SLAVE, this->addr) < 0) {
        exit(1);
    }

    this->thread_obj = new std::thread(&PT::loop,this);

}

int PT::recv() {
    char buf[4];
    if (read(file, buf, 4) < 0) { return -1; }

    float tmp;

    tmp = (PMAX - PMIN) / 14000.0f * (float)(((buf[0] << 8) | buf[1]) - 1000) + PMIN;
    if (tmp>-1&&tmp<350) { _pressure = tmp; }

    tmp = (200 / 2048.0f) * (float)((buf[2] << 3) | (buf[3] >> 5)) - 50.0f;
    if (tmp<125&&tmp>-40) { _temperature = tmp; }

    read(file, buf, 0);
    return 0;
}

void PT::loop() {
    while(this->is_alive){
        this->recv();
        std::this_thread::sleep_for(std::chrono::microseconds((const int)1e6/frequency));
    }
}
