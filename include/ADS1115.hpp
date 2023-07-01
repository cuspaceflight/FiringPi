#ifndef ADS1115_H
#define ADS1115_H

#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <thread>


// ADS1115 registers
#define ADS1115_REG_CONVERT 0x00
#define ADS1115_REG_CONFIG  0x01

// ADS1115 configuration values
#define ADS1115_CFG_MUX_DIFF_0_1   0x0000 // Differential P = AIN0, N = AIN1 (default)
#define ADS1115_CFG_MUX_DIFF_0_3   0x1000 // Differential P = AIN0, N = AIN3
#define ADS1115_CFG_MUX_DIFF_1_3   0x2000 // Differential P = AIN1, N = AIN3
#define ADS1115_CFG_MUX_DIFF_2_3   0x3000 // Differential P = AIN2, N = AIN3
#define ADS1115_CFG_MUX_SINGLE_0   0x4000 // Single-ended AIN0
#define ADS1115_CFG_MUX_SINGLE_1   0x5000 // Single-ended AIN1
#define ADS1115_CFG_MUX_SINGLE_2   0x6000 // Single-ended AIN2
#define ADS1115_CFG_MUX_SINGLE_3   0x7000 // Single-ended AIN3
#define ADS1115_CFG_PGA_6_144V     0x0000 // +/-6.144V range
#define ADS1115_CFG_PGA_4_096V     0x0200 // +/-4.096V range
#define ADS1115_CFG_PGA_2_048V     0x0400 // +/-2.048V range (default)
#define ADS1115_CFG_PGA_1_024V     0x0600 // +/-1.024V range
#define ADS1115_CFG_PGA_0_512V     0x0800 // +/-0.512V range
#define ADS1115_CFG_PGA_0_256V     0x0A00 // +/-0.256V range
#define ADS1115_CFD_MUX_INC        0x1000 // increment to next channel
#define ADS1115_CFD_MODE_SS        0x0100 // single shot mode
#define ADS1115_CFG_SPS_8          0x0000 // 8 SPS
#define ADS1115_CFG_SPS_16         0x0020 // 16 SPS
#define ADS1115_CFG_SPS_32         0x0040 // 32 SPS
#define ADS1115_CFG_SPS_64         0x0060 // 64 SPS
#define ADS1115_CFG_SPS_128        0x0080 // 128 SPS
#define ADS1115_CFG_SPS_250        0x00A0 // 250 SPS
#define ADS1115_CFG_SPS_475        0x00C0 // 475 SPS
#define ADS1115_CFG_SPS_860        0x00E0 // 860 SPS

#define ADS1115_FACTOR             32768.0

class ADS1115
{
public:
    ADS1115(uint8_t bus, uint8_t address, bool* hold);
    bool initialize();
    int16_t readConversion(void);
    uint16_t getConfig(void);
    float readChannel(size_t channel);
    int16_t getRawData(size_t channel);
    void kill(void);


private:
    int fd_;
    uint16_t config;
    uint8_t bus_;
    uint8_t address_;
    bool* hold;
    bool setAddr(void);
    bool writeRegister(uint8_t reg, uint16_t data);
    uint16_t readRegister(uint8_t reg);
    int16_t readRawData(size_t channel);
    void setSingleChannel(size_t channel);
    bool isBusy(void);
    void reset(void);
};

#endif  // ADS1115_H