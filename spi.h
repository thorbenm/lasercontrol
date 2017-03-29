#ifndef SPI_H
#define SPI_H

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <math.h>
#include <chrono>

class digital_analog_converter{
	public:
	digital_analog_converter(unsigned int, double , double, double, double, unsigned int);
	void voltage_in_range(double voltage);
	uint16_t voltage_to_code(double voltage);
	double code_to_voltage(uint16_t code);
	void transmit(uint16_t code = 0, uint8_t device = 3, unsigned int cs = 0);
	void transmit_voltage(double voltage = 0, uint8_t device = 3, unsigned int cs = 0);
	void fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int cs = 0);

	private:
	unsigned int bits;
	double min_voltage; // dac output at transmitting 0;
	double max_voltage;  // dac output at transmitting 2^bits (hightest value)   BEWARE ORIENTATION!!!
	double min_voltage_constrain;
	double max_voltage_constrain;
	unsigned int cs; 
	static int LDAC; //pi pin used for LDAC control
	static bool dac_setup;
};


double map(double, double, double, double, double);

double time_since(auto input);

#endif
