#include "../include/ADC.hpp"


ADC::ADC(int bus, int addr, int freq) : freq(freq), values(), is_alive(true) {
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    this->file = open(filename, O_RDWR);

    if (this->file < 0) {
        std::cerr << "could not open i2c bus " << bus << std::endl;
        exit(1);
    }

    if (ioctl(this->file, I2C_SLAVE, addr) < 0) {
        std::cerr << "could not communicate with i2c addr " << addr << " on bus " << bus << std::endl;
        exit(1);
    }

    this->thread_obj = new std::thread(&ADC::loop, this);
}

int ADC::recv() {
    auto start = std::chrono::system_clock::now();
    char buf[3];
    for (int i = 0; i < 3; i++) {
        buf[0] = 0x01;
        buf[1] = 0xC2 + (i << 4);
        buf[2] = 0x83;
        std::cerr<<"about to write to status "<<(std::chrono::system_clock::now()-start).count()<<std::endl;
        write(this->file, buf, 3);
        buf[0] = 0x00;
        std::cerr<<"about to write to pointer "<<(std::chrono::system_clock::now()-start).count()<<std::endl;
        write(this->file, buf ,1);
        std::cerr<<"about to read conversion "<<(std::chrono::system_clock::now()-start).count()<<std::endl;
        read(this->file, buf, 2);
        int out = (buf[0]<<4)+buf[1];

        // TODO need a decoding function for each sensor
        this->values[i] = (float)out;
    }
    return 0;
}

void ADC::loop() {
    while (this->is_alive) {
        this->recv();
        std::this_thread::sleep_for(std::chrono::microseconds((const int) 1e6 / this->freq));
    }
    close(this->file);
}
