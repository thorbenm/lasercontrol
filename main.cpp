#include "main.h"

int  digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_analog_converter dac(
		16,				//bits
		10.0,				//min_voltage (dac output at transmitting 0}
		-10.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		current_to_voltage(100.0),	//min_voltage_constrain
		0.001,				//max_voltage_contrain
		0				//Chip select
		);


double input = 0;
double old_input = 0;

void setup(){
	std::cout << "Input Current:\n" ; 
}

void loop(){
	std::cin >> input;
	dac.fade(current_to_voltage(old_input),current_to_voltage(input));
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

double current_to_voltage(double current){
	return - current * 0.05246298729 * 50.0 / 53.9; 
}

double voltage_to_current(double voltage){
	return voltage / current_to_voltage(1);
}