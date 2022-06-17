#pragma once

extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

#include "../include/defines.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include <array>

#define NUM_SERVOS 3

#define OSC_FREQ 25000000L
/* Register address and bit positions/masks for the devices internal registers */
#define MODE1 0x00
#define MODE1_ALLCALL_BIT 0
#define MODE1_ALLCALL_MASK ~(1 << MODE1_ALLCALL_BIT)
#define MODE1_SUB3_BIT 1
#define MODE1_SUB3_MASK ~(1 << MODE1_SUB3_BIT)
#define MODE1_SUB2_BIT 2
#define MODE1_SUB2_MASK ~(1 << MODE1_SUB2_BIT)
#define MODE1_SUB1_BIT 3
#define MODE1_SUB1_MASK ~(1 << MODE1_SUB1_BIT)
#define MODE1_SLEEP_BIT 4
#define MODE1_SLEEP_MASK ~(1 << MODE1_SLEEP_BIT)
#define MODE1_AI_BIT 5
#define MODE1_AI_MASK ~(1 << MODE1_AI_BIT)
#define MODE1_EXTCLK_BIT 6
#define MODE1_EXTCLK_MASK ~(1 << MODE1_EXTCLK_BIT)
#define MODE1_RESTART_BIT 7
#define MODE1_RESTART_MASK ~(1 << MODE1_RESTART_BIT)

#define MODE2 0x01
#define MODE2_OUTNE_BIT 0
#define MODE2_OUTNE_MASK ~(3 << MODE2_OUTNE_BIT)
#define MODE2_OUTDRV_BIT 2
#define MODE2_OUTDRV_MASK ~(1 << MODE2_OUTDRV_BIT)
#define MODE2_OCH_BIT 3
#define MODE2_OCH_MASK ~(1 << MODE2_OCH_BIT)
#define MODE2_INVRT_BIT 4
#define MODE2_INVRT_MASK ~(1 << MODE2_INVRT_BIT)
#define OCH_ACK 1
#define OCH_STOP 0
#define OUTDRV_TOTEM_POLE 1
#define OUTDRV_OPEN_DRAIN 1
#define OUTNE_LOW 0
#define OUTNE_HIGH 1
#define OUTNE_HIGH_IMPEDANCE 2

#define SUBADR1 0x02
#define SUBADR2 0x03
#define SUBADR3 0x04
#define ALLCALLADR 0x05
#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09
#define ALL_LED_ON_L 0xFA
#define ALL_LED_ON_H 0xFB
#define ALL_LED_OFF_L 0xFC
#define ALL_LED_OFF_H 0xFD
#define PRE_SCALE 0xFE

class Servo {
private:
    int file, freq, addr;
    static const int channels[NUM_SERVOS];

    void WriteReg(unsigned char reg, unsigned char data);

    void OutputOnTime(unsigned char chan, uint16_t time);

    void OutputOffTime(unsigned char chan, uint16_t time);

public:
    Servo(int bus, int address);

    int position;

    void write_position(unsigned char chan, uint16_t Pos);

    void write_positions(const std::array<uint16_t, NUM_SERVOS> *values);
};
