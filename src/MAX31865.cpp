#include "../include/MAX31865.hpp"

// MAX31865 register addresses

// SPI parameters
#define SPI_DEVICE      "/dev/spidev1.1"
#define SPI_SPEED       1000000     // 1 MHz
#define SPI_MODE        SPI_MODE_1  // SPI mode 0

MAX31865::MAX31865() {
    spi_fd = -1;
}

MAX31865::~MAX31865() {
    if (spi_fd != -1)
        close(spi_fd);
}

bool MAX31865::initialize() {
    // spi_fd = open(SPI_DEVICE, O_RDWR);
    // if (spi_fd < 0) {
    //     std::cout << "Failed to open SPI device." << std::endl;
    //     return false;
    // }

    // // Set SPI mode and speed
    // uint8_t spi_mode = 3; // SPI_MODE;

    // if (ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode) < 0) {
    //     std::cout << "Failed to set SPI mode rd." << std::endl;
    //     return false;
    // }
    // if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) < 0) {
    //     std::cout << "Failed to set SPI mode wr." << std::endl;
    //    return false;
    // }

    // uint32_t spi_speed = SPI_SPEED;
    // if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0) {
    //     std::cout << "Failed to set SPI speed." << std::endl;
    //     return false;
    // }
    // if (ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed) < 0) {
    //     std::cout << "Failed to set SPI speed." << std::endl;
    //     return false;
    // }

    // uint32_t spi_bits = 8;
    // if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) < 0) {
    //     std::cout << "Failed to set SPI bits." << std::endl;
    //     return false;
    // }
    // if (ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits) < 0) {
    //     std::cout << "Failed to set SPI bits." << std::endl;
    //     return false;
    // }

    // // Configure MAX31865
    // uint8_t cfg = readRegister(MAX31865_CONFIG_REG);
    // std::cout << "cfg POR = " << std::hex << (int) cfg << std::endl;
    // uint8_t msb = readRegister(MAX31865_FAULT_HIGH_MSB);
    // std::cout << "high fault msb POR = " << std::hex << (int) msb << std::endl;
    // writeRegister(MAX31865_CONFIG_REG_WR, 0b10010001); // Write 0x80 to configuration register
    // cfg = readRegister(MAX31865_CONFIG_REG);
    // writeRegister(MAX31865_FAULT_HIGH_MSB,0xFE);
    // msb = readRegister(MAX31865_FAULT_HIGH_MSB);
    // std::cout << "cfg = " << std::hex << (int) cfg << std::endl;
    // std::cout << "msb = " << std::hex << (int) msb << std::endl;

    SPI *mySPI = NULL;

    spi_config.mode=3;
    spi_config.speed=1000000;
    spi_config.delay=0;
    spi_config.bits_per_word=8;

    mySPI=new SPI("/dev/spidev1.0",&spi_config);
    if (mySPI->begin())
    {
        return true;
    } else {
        return false;
    }
}

void MAX31865::configure() {
    uint8_t cfg = readRegister(MAX31865_CONFIG_REG);
    std::cout << "cfg POR = " << std::hex << (int) cfg << std::endl;
    writeRegister(MAX31865_CONFIG_REG_WR, 0b10010001); // Write 0x80 to configuration register
    cfg = readRegister(MAX31865_CONFIG_REG);
    std::cout << "cfg = " << std::hex << (int) cfg << std::endl;
}

float MAX31865::readTemperature() {
    uint8_t msb = readRegister(MAX31865_RTD_MSB_REG);
    uint8_t lsb = readRegister(MAX31865_RTD_LSB_REG);
    int16_t temp_raw = (msb << 8) | lsb;
    float temperature = temp_raw / 32.0f;

    return temperature;
}

uint8_t MAX31865::readRegister(uint8_t reg) {
    uint8_t tx_buf[2] = {reg, 0x00};
    uint8_t rx_buf[2] = {0x00,0x00};
    std::cout << "rR initial: tx_buf: " << std::hex << (int) tx_buf[0] << std::hex << (int) tx_buf[1] << std::endl;
    std::cout << "            rx_buf: " << std::hex << (int) rx_buf[0] << std::hex << (int) rx_buf[1] << std::endl;

    // struct spi_ioc_transfer spi;
    // spi.tx_buf = (unsigned long)tx_buf;
    // spi.rx_buf = (unsigned long)rx_buf;
    // spi.len = 2;
    // spi.speed_hz = SPI_SPEED;
    // spi.bits_per_word = 8;
    // spi.delay_usecs = 0;
    // spi.cs_change = 0;

    // ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);

    // std::cout << "rR final  : tx_buf: " << std::hex << (int) tx_buf[0] << std::hex << (int) tx_buf[1] << std::endl;
    // std::cout << "            rx_buf: " << std::hex << (int) rx_buf[0] << std::hex << (int) rx_buf[1] << std::endl;

    // std::cout << "readRegister" << std::hex << (int) rx_buf[1] << std::endl;
    // return rx_buf[1];

    mySPI->xfer(tx_buf, 2, rx_buf, 2);
    std::cout << "rR final  : tx_buf: " << std::hex << (int) tx_buf[0] << std::hex << (int) tx_buf[1] << std::endl;
    std::cout << "            rx_buf: " << std::hex << (int) rx_buf[0] << std::hex << (int) rx_buf[1] << std::endl;
    return rx_buf[1];
}

void MAX31865::writeRegister(uint8_t reg, uint8_t value) {
    std::cout << "writeRegister: " << std::hex << (int) reg << "   " << std::hex << (int)  value << std::endl;
    uint8_t tx_buf[2] = {reg, value};
    uint8_t rx_buf[2] = {0x00, 0x00};
    // std::cout << "wR initial: tx_buf: " << std::hex << (int) tx_buf[0] << std::hex << (int) tx_buf[1] << std::endl;
    // std::cout << "            rx_buf: " << std::hex << (int) rx_buf[0] << std::hex << (int) rx_buf[1] << std::endl;

    // struct spi_ioc_transfer spi;
    // spi.tx_buf = (unsigned long)tx_buf;
    // spi.rx_buf = (unsigned long)rx_buf;
    // spi.len = 2;
    // spi.speed_hz = SPI_SPEED;
    // spi.bits_per_word = 8;
    // spi.delay_usecs = 0;
    // spi.cs_change = 0;

    // ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
    mySPI->xfer(tx_buf, 2, rx_buf, 2);
    std::cout << "wR final  : tx_buf: " << std::hex << (int) tx_buf[0] << std::hex << (int) tx_buf[1] << std::endl;
    std::cout << "            rx_buf: " << std::hex << (int) rx_buf[0] << std::hex << (int) rx_buf[1] << std::endl;
}
