#include "../include/ADS1115.hpp"

ADS1115::ADS1115(uint8_t bus, uint8_t address, bool* hold) : fd_(-1), bus_(bus), address_(address), hold(hold)
{

}

void ADS1115::reset(void)
{
    while(*hold) {}
    *hold = true;
    setAddr();
    uint8_t buf[1];
    buf[1] = 0x06;
    if (write(fd_, buf, 1) != 1)
    {
        std::cerr << "reset failed." << std::endl;
    }
    *hold = false;
}

bool ADS1115::initialize()
{
    char filename[20];
    snprintf(filename, 19, "/dev/i2c-%d", bus_);
    fd_ = open(filename, O_RDWR);
    if (fd_ == -1)
    {
        std::cerr << "Failed to open the I2C device." << std::endl;
        return false;
    }

    reset();
    uint16_t cfg = readRegister(ADS1115_REG_CONFIG);
    std::cerr << "reset config:" << std::hex << cfg << std::endl;
    cfg &= 0x000F;
    cfg |= (ADS1115_CFG_MUX_SINGLE_0 | ADS1115_CFG_PGA_6_144V | ADS1115_CFD_MODE_SS | ADS1115_CFG_SPS_128);
    config = cfg;
    std::cerr << "intended config:" << std::hex << cfg << std::endl;
    writeRegister(ADS1115_REG_CONFIG, cfg);
    cfg = readRegister(ADS1115_REG_CONFIG);
    std::cerr << "initial config:" << std::hex << cfg << std::endl;

    return true;
}

int16_t ADS1115::readConversion()
{
    if (ioctl(fd_, I2C_SLAVE, address_) < 0)
    {
        std::cerr << "Failed to set the I2C slave address." << std::endl;
    }
    uint8_t buf[2];
    if (read(fd_, buf, 2) != 2)
    {
        std::cerr << "Failed to read conversion result." << std::endl;
        return -1;
    }
    int16_t value = (buf[0] << 8) | buf[1];
    return value;
}


bool ADS1115::setAddr(void)
{
    if (ioctl(fd_, I2C_SLAVE, address_) < 0)
    {
        std::cerr << "Failed to set the I2C slave address." << std::endl;
        return false;
    }
    else
    {
        return true;
    }
}

bool ADS1115::writeRegister(uint8_t reg, uint16_t data)
{
    while(*hold) {}
    *hold = true;
    setAddr();
    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = data >> 8;
    buf[2] = data & 0xFF;
    if (write(fd_, buf, 3) != 3)
    {
        std::cerr << "writeRegister failed." << std::endl;
        *hold = false;
        return false;
    }
    else
    {
        *hold = false;
        return true;
    }
}

uint16_t ADS1115::readRegister(uint8_t reg)
{    
    while(*hold) {}
    *hold = true;
    setAddr();
    uint8_t buf[2];
    buf[0] = reg;
    if (write(fd_, buf, 1) != 1)
    {
        std::cerr << "selecting register to read failed." << std::endl;
        *hold = false;
        return false;
    }
    if (read(fd_, buf, 2) != 2)
    {
        std::cerr << "readRegister failed." << std::endl;
        *hold = false;
        return -1;
    }
    uint16_t value = (buf[0] << 8) | buf[1];
    return value;
}

void ADS1115::setSingleChannel(size_t channel)
{
    uint16_t mux = (uint16_t)(ADS1115_CFG_MUX_SINGLE_0 + channel*ADS1115_CFD_MUX_INC);
    // std::cerr << "setSingleChannel" << std::endl;
    uint16_t cfg = readRegister(ADS1115_REG_CONFIG);
    std::cerr << std::hex << cfg << std::endl;
    cfg = config & ~(0xF000);
    std::cerr << "setting cfg = " << std::hex << cfg << std::endl;
    std::cerr << "config = " << std::hex << config << std::endl;
    //cfg = config;
    cfg |= mux;
    std::cerr << "setting cfg = " << std::hex << cfg << std::endl;
    writeRegister(ADS1115_REG_CONFIG, cfg);
}

uint16_t ADS1115::getConfig(void)
{
    return readRegister(ADS1115_REG_CONFIG);
}

int16_t ADS1115::readRawData(size_t channel)
{
    setSingleChannel(channel);
    uint16_t cfg = readRegister(ADS1115_REG_CONFIG);

    std::cerr << "readRawData config:" << std::hex << cfg << std::endl;
    std::cerr << "config:" << std::hex << config << std::endl;
    cfg |= (1<<15);
    writeRegister(ADS1115_REG_CONFIG, cfg);
    while (isBusy()) {}
    int16_t value = readRegister(ADS1115_REG_CONVERT);
    return value;
}

int16_t ADS1115::getRawData(size_t channel)
{
    return readRawData(channel);
}

bool ADS1115::isBusy(void)
{
    uint16_t cfg = readRegister(ADS1115_REG_CONFIG);
    return(!((cfg>>15)&1));
}

float ADS1115::readChannel(size_t channel)
{
    int16_t rawResult = readRawData(channel);
    float result = (float) rawResult * (1.0 /ADS1115_FACTOR) * 6.144;
    return result;
}

void ADS1115::kill(void)
{
    close(fd_);
}