#include "../include/ADS1115.hpp"
#include <unistd.h>

int main()
{
    uint8_t adsBus = 1;  // I2C bus number
    uint8_t address = 0x48; // Address of the ADS1115

    ADS1115 adc(adsBus, address);
    bool init = adc.initialize();
    if (!init)
    {
        
        return 1;
    }
    //uint16_t value = adc.getRawData(0);

    while (true) { 
        // Read and print the conversion result
        float value = adc.readChannel(0);
        // uint16_t cfg = adc.getConfig();
        std::cout << "Conversion result 1 : " << value << std::endl;
        value = adc.readChannel(1);
        // uint16_t cfg = adc.getConfig();
        std::cout << "Conversion result 2 : " << value << std::endl;
        // std::cout << "Config register: " <<  std::hex << cfg << std::endl;
        sleep(1);
    }
    return 0;
}