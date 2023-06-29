#ifndef MAX31865_H
#define MAX31865_H

#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "../include/spidev_lib.hpp"

#define MAX31865_CONFIG          0b10010000 // 0x00

#define MAX31865_CONFIG_REG      0x00
#define MAX31865_RTD_MSB_REG     0x01
#define MAX31865_RTD_LSB_REG     0x02
#define MAX31865_CONFIG_REG_WR   0x80
#define MAX31865_FAULT_HIGH_MSB  0x03
#define MAX31865_FAULT_HIGH_LSB  0x04


class MAX31865 {
public:
    MAX31865();
    ~MAX31865();

    bool initialize();
    float readTemperature();

private:
    int spi_fd;
    spi_config_t spi_config;
    SPI* mySPI;

    uint8_t readRegister(uint8_t reg);
    void configure(void);
    void writeRegister(uint8_t reg, uint8_t value);
};

#endif  // MAX31865_H
