#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <chrono>

// header:

uint16_t voltage_to_code(double voltage);

double time_since(auto input);

bool transmit(uint16_t code = 0, uint8_t device = 3, unsigned int channel = 0);

bool transmit_voltage(double voltage = 0, uint8_t device = 3, unsigned int channel = 0);

bool fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);

bool sine_fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);

double current_to_voltage(double);

double voltage_to_current(double voltage);


//contrain:
double min_voltage = current_to_voltage(50);
double max_voltage = .001;

//DAC Parameter:
double dac_min_voltage = 10; // dac output at minimum input
double dac_max_voltage = -10;  // dac output at maximum input   BEWARE ORIENTATION!!!
const int LDAC = 6; //pi pin used for LDAC control


double voltage_step = abs(dac_min_voltage - dac_max_voltage) / (double) 0xFFFF;







void setup(){
	wiringPiSPISetup (0, 32000000) ;
	wiringPiSetup () ;
	pinMode (LDAC, OUTPUT) ;
	digitalWrite (LDAC,HIGH);

	transmit_voltage(0);

	std::cout << "Input Current, please:\n" ; 
}

//int main (void){
//	setup();
//	auto start = std::chrono::high_resolution_clock::now();
//
//	double input = 0;
//	double old_input = 0;
//
//	for (;;){
//		std::cin >> input;
//		if(!(sine_fade(current_to_voltage(old_input),current_to_voltage(input)))){
//			break;
//		}
//		old_input = input;
//	}
//	transmit_voltage(0);
//	return 0 ;
//}

bool voltage_out_of_range(double voltage){
	if(max_voltage < voltage){
		std::cerr << "Voltage / Current out of Range\nProgramme will be closed \n";
		return 1;
	}
	else if(voltage < min_voltage){
		std::cerr << "Voltage / Current out of Range\nProgramme will be closed \n";
		return 1;
	}
	else
		return 0;
}

uint16_t voltage_to_code(double voltage){
	double value = voltage * 0xFFFF / (dac_max_voltage - dac_min_voltage) + 0xFFFF - 0xFFFF / (dac_max_voltage - dac_min_voltage) * dac_max_voltage; 
	uint16_t code = (uint16_t) value;
	return code;
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

bool transmit(uint16_t code, uint8_t device, unsigned int channel){
	uint8_t code1 = code >> 8;
	uint8_t code2 = code & 0xFF;
	wiringPiSPIDataRW (channel, (unsigned char*)&device, sizeof(device));
	wiringPiSPIDataRW (channel, (unsigned char*)&code1, sizeof(code1));
	wiringPiSPIDataRW (channel, (unsigned char*)&code2, sizeof(code2));
	digitalWrite (LDAC,LOW);
//	delayMicroseconds(1);
	digitalWrite (LDAC,HIGH);
	return 1;
}

bool transmit_voltage(double voltage, uint8_t device, unsigned int channel){
	if(voltage_out_of_range(voltage)){
		return 0;
	}
	transmit(voltage_to_code(voltage), device, channel);
	return 1;
}

bool fade(double from, double to, double time, uint8_t device, unsigned int channel){
	if(voltage_out_of_range(to)){
		return 0;
	}
	if(voltage_out_of_range(from)){
		return 0;
	}
	auto function_start = std::chrono::high_resolution_clock::now(); 
	double voltage;
	if(from < to){
		do{
			voltage = from + (to - from) / time * time_since(function_start); 
			if(!(transmit_voltage(voltage, device, channel))){
				return 0;
			}
		}while(voltage < to);
	}
	else{
		do{
			voltage = from + (to - from) / time * time_since(function_start); 
			if(!(transmit_voltage(voltage, device, channel))){
				return 0;
			}
		}while(to < voltage);
	}
	return transmit_voltage(to, device, channel);
}

bool sine_fade(double from, double to, double time, uint8_t device, unsigned int channel){
	const double pi = 3.14;
	double amplitude = from - (to + from) / 2.0 ;
	double offset = (from + to) / 2.0;

	if(voltage_out_of_range(to)){
		return 0;
	}
	if(voltage_out_of_range(from)){
		return 0;
	}
	auto function_start = std::chrono::high_resolution_clock::now(); 
	double voltage = 0;
	while(time_since(function_start) < time){
		voltage = offset + amplitude * cos (pi / time * time_since(function_start)); 
		if(!(transmit_voltage(voltage, device, channel))){
			return 0;
		}
	}
	return transmit_voltage(to, device, channel);
}


double current_to_voltage(double current){
	return - current * 0.05246298729; 
}

double voltage_to_current(double voltage){
	return voltage / current_to_voltage(1);
}
