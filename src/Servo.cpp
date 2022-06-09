#include "../include/Servo.hpp"

const int Relay::channels[NUM_SERVOS] = {0, 1}

Servo::Servo(int bus, int address, int ) : {
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

    // Set PWM frequency
    unsigned char data = (OSC_FREQ / (4096L * SERVO_FREQ)) - 1;
    WriteReg(PRE_SCALE, data);

}

void Servo::WriteReg(unsigned char reg, unsigned char data) {
    unsigned char buffer[2];
    buffer[0] = reg;
    buffer[1] = data;
    int length = 2;
    if ( write(this->file, buffer, length) != length ) {
        std::cerr << "Failed to write to the i2c bus.\n" << std::endl;
    }
}

/* Sets position of servo

   Chan is the PWM output pin to change. Valid values are
		0 (PWM output pin 0) to 15 (PWM output pin 15)
		
   Pos is the required position of the servo. Valid values are
		0 (servo fully anti-clockwise) to 480 (servo fully clockwise)
		
	The maximum value is based in the default servo SERVO_TRIM_MAX and SERVO_TRIM_MIN trim settings found in the defines.hpp header file.
	These default to 590 for SERVO_TRIM_MAX and 110 for SERVO_TRIM_MIN.
	Thay can be adjusted to match the minimum and maximum end-stop positions of the servos. If these values are changed then the new max value for Pos can 
	be calculated as (SERVO_TRIM_MAX - SERVO_TRIM_MIN) */
void Servo::write_position(unsigned char reg, uint16_t Pos) {
    this->position = Pos;
    Pos += SERVO_TRIM_MIN;
    if (Pos > SERVO_TRIM_MAX) {
        Pos = SERVO_TRIM_MAX;
    }
    OutputOnTime(chan, 0);
    OutputOffTime(chan, Pos);
}

void Servo::write_positions(const std::array<int, NUM_SERVOS> *values) {
    for (int i = 0; auto output: *values) {
        write_position(channels[i], output);
        i++;
    }
}


void Servo::OutputOnTime(unsigned char chan, uint16_t time) {
    buffer[0] = LED0_ON_L + (chan << 2);
    buffer[1] = time & 0x00FF;
    buffer[2] = time >> 8;
    int length = 3;
    if ( write(this->file, buffer, length) != length ) {
        std::cerr << "Failed to set servo off time.\n" << std::endl;
    }
}

void Servo::OutputOffTime(unsigned char chan, uint16_t time) {
    buffer[0] = LED0_OFF_L + (chan << 2);
    buffer[1] = time & 0x00FF;
    buffer[2] = time >> 8;
    int length = 3;
    if ( write(this->file, buffer, length) != length ) {
        std::cerr << "Failed to set servo off time.\n" << std::endl;
    }
}
