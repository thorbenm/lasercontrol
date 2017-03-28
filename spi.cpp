#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <chrono>


// header:

double map(double,double,double,double,double);

double time_since(auto input);

//void fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);


double abs(double input){
	return (input > 0.0)? input: -input;
}

//contrain:
//double min_voltage = current_to_voltage(100);
//double max_voltage = .001;

//DAC Parameter:
//int bits = 16;
//double dac_min_voltage = 10; // dac output at transmitting 0;
//double dac_max_voltage = -10;  // dac output at transmitting 2^bits (hightest value)   BEWARE ORIENTATION!!!
//const int LDAC = 6; //pi pin used for LDAC control



//double voltage_step = abs(dac_min_voltage - dac_max_voltage) / (double) pow(2.0, (double) bits);
//double current_step = abs(voltage_to_current(voltage_step));

double input = 0;
double old_input = 0;

class digital_analog_converter{
	public:
	digital_analog_converter(unsigned int, double , double, double, double, unsigned int, unsigned int);
	void voltage_in_range(double voltage);
	uint16_t voltage_to_code(double voltage);
	double code_to_voltage(uint16_t code);
	double current_to_voltage(double);
	double voltage_to_current(double voltage);
	void transmit(uint16_t code = 0, uint8_t device = 3, unsigned int channel = 0);
	void transmit_voltage(double voltage = 0, uint8_t device = 3, unsigned int channel = 0);
	void sine_fade(double from, double to, double time = 1, uint8_t device = 3, unsigned int channel = 0);

	private:
	unsigned int bits;
	double min_voltage; // dac output at transmitting 0;
	double max_voltage;  // dac output at transmitting 2^bits (hightest value)   BEWARE ORIENTATION!!!
	double min_voltage_constrain;
	double max_voltage_constrain;
	unsigned int LDAC; //pi pin used for LDAC control
	unsigned int channel; 
	static bool wiringpi_setup;
};

bool digital_analog_converter::wiringpi_setup = false;

digital_analog_converter dac(
		16,  //bits
		10,  //min_voltage (dac output at transmitting 0}
		-10, //max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		-5,  //min_voltage_constrain
		.001,   //max_voltage_contrain
		6,   //LDAC pin
		0    //CS channel
		);

void setup(){
	std::cout << "Input Current:\n" ; 
}

void loop(){
	std::cin >> input;
	dac.sine_fade(dac.current_to_voltage(old_input),dac.current_to_voltage(input));
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

digital_analog_converter::digital_analog_converter(unsigned int b, double minv, double maxv, double minvc, double maxvc, unsigned int l, unsigned int c){

	bits = b;
	min_voltage = minv;
	max_voltage = maxv;
	min_voltage_constrain = maxvc;
	max_voltage_constrain = maxvc;
	LDAC = l;
	channel = c; 

	if (wiringpi_setup == false){
		wiringPiSPISetup (0, 32000000) ;
		wiringPiSetup () ;
		wiringpi_setup == true;
	}

	pinMode (LDAC, OUTPUT) ;
	digitalWrite (LDAC,HIGH);

//	transmit_voltage(0,3,channel)
}

void digital_analog_converter::voltage_in_range(double voltage){
	if(max_voltage_constrain < voltage || voltage < min_voltage_constrain){
		std::cerr << "Voltage / Current out of Range\nProgramme will be closed\n";
//		transmit_voltage(0);
		exit(0);
	}
}

uint16_t digital_analog_converter::voltage_to_code(double voltage){
	double value = map(voltage, min_voltage_constrain, max_voltage_constrain, 0.0 , pow(2.0, (double) bits) - 1.0); 
	uint16_t code = (uint16_t) value;
	return code;
}

double digital_analog_converter::code_to_voltage(uint16_t code){
	double value = (double) code;
	value = map(value, 0.0 , pow(2.0, (double) bits) - 1.0, min_voltage_constrain , max_voltage_constrain); 
	return value;
}

double digital_analog_converter::current_to_voltage(double current){
	return - current * 0.05246298729 * 50.0 / 53.9; 
}

double digital_analog_converter::voltage_to_current(double voltage){
	return voltage / current_to_voltage(1);
}

void digital_analog_converter::transmit(uint16_t code, uint8_t device, unsigned int channel){
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

void digital_analog_converter::transmit_voltage(double voltage, uint8_t device, unsigned int channel){
	transmit(voltage_to_code(voltage), device, channel);
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

void digital_analog_converter::sine_fade(double from, double to, double time, uint8_t device, unsigned int channel){
	voltage_in_range(to);
	voltage_in_range(from);

	const double pi = 3.14;
	double amplitude = from - (to + from) / 2.0 ;
	double offset = (from + to) / 2.0;

	auto function_start = std::chrono::high_resolution_clock::now(); 
	double voltage = 0;
	while(time_since(function_start) < time){
		voltage = offset + amplitude * cos (pi / time * time_since(function_start)); 
		transmit_voltage(voltage, device, channel);
	}
	transmit_voltage(to, device, channel);
}


