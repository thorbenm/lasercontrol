#include "spi.h"
#include "main.h"

bool digital_analog_converter::dac_setup = false;

digital_analog_converter::digital_analog_converter(unsigned int b, double minv, double maxv, double minvc, double maxvc, unsigned int c){

	bits = b;
	min_voltage = minv;
	max_voltage = maxv;
	min_voltage_constrain = minvc;
	max_voltage_constrain = maxvc;
	cs = c; 

	if (dac_setup == false){
		wiringPiSPISetup (0, 32000000) ;
		wiringPiSetup () ;
		if(LDAC >= 0){
			pinMode (LDAC, OUTPUT) ;
			digitalWrite (LDAC,HIGH);
		}
		dac_setup == true;
	}


	transmit_voltage();
}

void digital_analog_converter::voltage_in_range(double voltage){
	if(max_voltage_constrain < voltage || voltage < min_voltage_constrain){
		std::cerr << "Voltage / Current out of Range\nProgramme will be closed\n";
		transmit_voltage();
		exit(0);
	}
}

uint16_t digital_analog_converter::voltage_to_code(double voltage){
	double value = map(voltage, min_voltage, max_voltage, 0.0 , pow(2.0, (double) bits) - 1.0); 
	uint16_t code = (uint16_t) value;
	return code;
}

double digital_analog_converter::code_to_voltage(uint16_t code){
	double value = (double) code;
	value = map(value, 0.0 , pow(2.0, (double) bits) - 1.0, min_voltage , max_voltage); 
	return value;
}

void digital_analog_converter::fade(double from, double to, double time, uint8_t device, unsigned int cs){
	voltage_in_range(to);
	voltage_in_range(from);

	const double pi = 3.14;
	double amplitude = from - (to + from) / 2.0 ;
	double offset = (from + to) / 2.0;

	auto function_start = std::chrono::high_resolution_clock::now(); 
	double voltage = 0;
	while(time_since(function_start) < time){
		voltage = offset + amplitude * cos (pi / time * time_since(function_start)); 
		transmit_voltage(voltage, device, cs);
	}
	transmit_voltage(to, device, cs);
}

void digital_analog_converter::transmit(uint16_t code, uint8_t device, unsigned int cs){
	if(bits != 16){
		std::cerr << "Programme is hardcoded for bits = 16";
		exit(0);
	}
	voltage_in_range(code_to_voltage(code));
	uint8_t code1 = code >> 8;
	uint8_t code2 = code & 0xFF;
	wiringPiSPIDataRW (cs, (unsigned char*)&device, sizeof(device));
	wiringPiSPIDataRW (cs, (unsigned char*)&code1, sizeof(code1));
	wiringPiSPIDataRW (cs, (unsigned char*)&code2, sizeof(code2));
	if(LDAC >= 0){
		digitalWrite (LDAC,LOW);
//		delayMicroseconds(1);
		digitalWrite (LDAC,HIGH);
	}
}

void digital_analog_converter::transmit_voltage(double voltage, uint8_t device, unsigned int cs){
	transmit(voltage_to_code(voltage), device, cs);
}

double map(double value, double fromLow, double fromHigh, double toLow, double toHigh){
	if(fromLow == fromHigh || toLow == toHigh){
		std::cerr << "mapping not possible\n";
		exit(0);
	}
	return value * (toHigh - toLow) / (fromHigh - fromLow) + toHigh - (toHigh - toLow) / (fromHigh - fromLow) * fromHigh; 
}

double time_since(auto input){
	// to get time since programme start put
	// auto start_time = std::chrono::high_resolution_clock::now(); 
	// at the beginning of your programme
	//
	// run as : time_since(start)
	auto diff = std::chrono::high_resolution_clock::now() - input; // get difference 
	auto nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(diff);
	double time1 = (double) nsec.count() / 1.0e9 ; 
	return time1;
}
