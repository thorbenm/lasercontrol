#include "main.h"

int digital_analog_converter::LDAC = 6; //pi pin used for LDAC control (set negative to turn off LDAC)

digital_analog_converter dac(
		32.0 * MHz,			//clock speed
		16,				//bits
		10.0,				//min_voltage (dac output at transmitting 0}
		-10.0,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		current_to_voltage(100.0),	//min_voltage_constrain
		0.001,				//max_voltage_contrain
		0				//Chip select
		);

analog_digital_converter mcp(
		0.5 * MHz,			//clock speed
		10,				//bits
		0.0,				//min_voltage (dac output at transmitting 0}
		3.3,				//max_voltage (dac output at transmitting 2^bits-1 BEWARE ORIENTATION!!!)
		1				//Chip select
		);


const double current_step = fabs(voltage_to_current(dac.voltage_step()));
double value = 0;
double current = 50;

double target = 0;

void setup(){
//	std::cout << "Input Current:\n" ; 
//	std::cout << "voltage_step = " << dac.voltage_step() << std::endl;
//	std::cout << "current_step = " << current_step << std::endl;
}

void loop(){
	std::cout << std::endl;
	value = mcp.read_voltage();
	std::cout << "value: " << value * 1000.0 << " mV" << std::endl;
	if(value < target){
		current += current_step;
		std::cout << "=> current++ \n";
	}
	if(value > target){
		current -= current_step;
		std::cout << "=> current-- \n";
	}
	dac.transmit_voltage(current_to_voltage(current));
	std::cout << "current = " << current << " mA" << std::endl;
	delay(5);
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
