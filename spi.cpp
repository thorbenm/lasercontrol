#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <chrono>


// header:

double map(double,double,double,double,double);

uint16_t voltage_to_code(double voltage);

double code_to_voltage(uint16_t code);

double time_since(auto input);

void transmit(uint16_t code = 0, uint8_t device = 3, unsigned int channel = 0);

void transmit_voltage(double voltage = 0, uint8_t device = 3, unsigned int channel = 0);

//void fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);

void sine_fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);

double current_to_voltage(double);

double voltage_to_current(double voltage);

double abs(double input){
	return (input > 0.0)? input: -input;
}


//contrain:
double min_voltage = current_to_voltage(100);
double max_voltage = .001;

//DAC Parameter:
int bits = 16;
double dac_min_voltage = 10; // dac output at transmitting 0;
double dac_max_voltage = -10;  // dac output at transmitting 2^bits (hightest value)   BEWARE ORIENTATION!!!
const int LDAC = 6; //pi pin used for LDAC control



double resolution = pow(2.0, (double) bits) - 1.0;
double voltage_step = abs(dac_min_voltage - dac_max_voltage) / (double) pow(2.0, (double) bits);
double current_step = abs(voltage_to_current(voltage_step));

double input = 0;
double old_input = 0;

void setup(){
	wiringPiSPISetup (0, 32000000) ;
	wiringPiSetup () ;
	pinMode (LDAC, OUTPUT) ;
	digitalWrite (LDAC,HIGH);

	transmit_voltage(0);

	std::cout << "Input Current:\n" ; 
}

void loop(){
	std::cin >> input;
	sine_fade(current_to_voltage(old_input),current_to_voltage(input));
	old_input = input;
}

int main (void){
	auto start = std::chrono::high_resolution_clock::now();

	setup();

	for (;;){
		loop();
	}
	return 0 ;
}

void voltage_in_range(double voltage){
	if(max_voltage < voltage || voltage < min_voltage){
		std::cerr << "Voltage / Current out of Range\nProgramme will be closed \n";
		transmit_voltage(0);
		exit(0);
	}
}

double map(double value, double fromLow, double fromHigh, double toLow, double toHigh){
	if(fromLow == fromHigh){
		std::cerr << "mapping not possible\n";
		exit(0);
	}
	if(toLow == toHigh){
		std::cerr << "mapping not possible\n";
		exit(0);
	}
	return value * (toHigh - toLow) / (fromHigh - fromLow) + toHigh - (toHigh - toLow) / (fromHigh - fromLow) * fromHigh; 
}

uint16_t voltage_to_code(double voltage){
	double value = map(voltage, dac_min_voltage, dac_max_voltage, 0.0 , resolution); 
	uint16_t code = (uint16_t) value;
	return code;
}

double code_to_voltage(uint16_t code){
	double value = (double) code;
	value = map(value, 0.0 , resolution, dac_min_voltage, dac_max_voltage); 
	return value;
}


double time_since(auto input){
	// to get time since programme start put
	// auto start_time = std::chrono::high_resolution_clock::now(); 
	// at the beginning of your programme
	//
	// run as : time_since(start)
	auto diff = std::chrono::high_resolution_clock::now() - input; // get difference 
	auto nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
	double time1 = (double) nsec.count() / 1000000000.0 ; 
	return time1;
}

void transmit(uint16_t code, uint8_t device, unsigned int channel){
	voltage_in_range(code_to_voltage(code));
	uint8_t code1 = code >> 8;
	uint8_t code2 = code & 0xFF;
	wiringPiSPIDataRW (channel, (unsigned char*)&device, sizeof(device));
	wiringPiSPIDataRW (channel, (unsigned char*)&code1, sizeof(code1));
	wiringPiSPIDataRW (channel, (unsigned char*)&code2, sizeof(code2));
	digitalWrite (LDAC,LOW);
//	delayMicroseconds(1);
	digitalWrite (LDAC,HIGH);
}

void transmit_voltage(double voltage, uint8_t device, unsigned int channel){
	transmit(voltage_to_code(voltage), device, channel);
}

//void fade(double from, double to, double time, uint8_t device, unsigned int channel){
//	voltage_in_range(from);
//	voltage_in_range(to);
//	auto function_start = std::chrono::high_resolution_clock::now(); 
//	double voltage;
//	if(from < to){
//		do{
//			voltage = from + (to - from) / time * time_since(function_start); 
//			if(!(transmit_voltage(voltage, device, channel))){
//				return 0;
//			}
//		}while(voltage < to);
//	}
//	else{
//		do{
//			voltage = from + (to - from) / time * time_since(function_start); 
//			if(!(transmit_voltage(voltage, device, channel))){
//				return 0;
//			}
//		}while(to < voltage);
//	}
//	transmit_voltage(to, device, channel);
//}

void sine_fade(double from, double to, double time, uint8_t device, unsigned int channel){
	const double pi = 3.14;
	double amplitude = from - (to + from) / 2.0 ;
	double offset = (from + to) / 2.0;

	voltage_in_range(to);
	voltage_in_range(from);
	auto function_start = std::chrono::high_resolution_clock::now(); 
	double voltage = 0;
	while(time_since(function_start) < time){
		voltage = offset + amplitude * cos (pi / time * time_since(function_start)); 
		transmit_voltage(voltage, device, channel);
	}
	transmit_voltage(to, device, channel);
}


double current_to_voltage(double current){
	return - current * 0.05246298729 * 50.0 / 53.9; 
}

double voltage_to_current(double voltage){
	return voltage / current_to_voltage(1);
}
