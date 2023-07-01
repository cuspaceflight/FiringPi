#include "PT.hpp"

PT::PT(int bus, int address, int frequency, bool* hold)
    : pressure(), temperature(), addr(address),
    freq(frequency), is_alive(true), hold(hold) {

    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    this->file = open(filename, O_RDWR);

    if (this->file < 0) {
        std::cerr << "could not open i2c bus " << bus << std::endl;
        exit(1);
    }

    std::cerr << "pt init" << (int) hold << "    " << (int) *hold << std::endl;
    while(*hold) {}
    *hold = true;
    if (ioctl(this->file, I2C_SLAVE, this->addr) < 0) {
        std::cerr << "could not communicate with i2c addr " << addr << " on bus " << bus << std::endl;
        exit(1);
    }
    *hold = false;
    this->thread_obj = new std::thread(&PT::loop, this);
    std::cerr << "pt init finished" << std::endl;

}

bool PT::setAddr(void)
{
    if (ioctl(this->file, I2C_SLAVE, this->addr) < 0) 
    {
        std::cerr << "Failed to set the I2C slave address." << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

int PT::recv() {
    while(*hold) {}
    *hold = true;
    setAddr();
    char buf[4];
    if (read(file, buf, 4) != 4) { return -1; }
    *hold = false;
    switch ((buf[0] & 0xc0)) {
        case 0x0 << 6:
            break;
        case 0x1 << 6:
            return -1;
        case 0x2 << 6:
            return 0;
        case 0x3 << 6:
            return -1;
    }

    float tmp;

    tmp = (PMAX - PMIN) / 14000.0f * (float) (((buf[0] << 8) | buf[1]) - 1000) + PMIN;
    if (tmp > -1 && tmp < 350) { pressure*=1-k_filter; pressure += k_filter*tmp; }

    tmp = (200 / 2048.0f) * (float) ((buf[2] << 3) | (buf[3] >> 5)) - 50.0f;
    if (tmp < 125 && tmp > -40) { temperature*=1-k_filter; temperature += k_filter*tmp; }
    read(file, buf, 0);
    return 0;


}

void PT::loop() {
    while (this->is_alive) {
        this->recv();
        std::this_thread::sleep_for(std::chrono::microseconds((const int) 1e6 / freq));
    }
    close(this->file);
}
