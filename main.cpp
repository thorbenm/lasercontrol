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


const double current_step = fabs(voltage_to_current(dac.get_voltage_step()));
double value = 0.0;
double current = 0.0;

double target = 1.0;

void setup(){
	std::cout << "choose staring value for current [mA]: " ; 
	std::cin >> current; 
}

void loop(){
	value = mcp.read_voltage();
	if(value < target){
		current += current_step;
	}
	if(value > target){
		current -= current_step;
	}
	dac.transmit_voltage(current_to_voltage(current));
	delay(5);
}

double every=1.0; // seconds
void routine(){
	std::cout << std::endl;
	std::cout << "value =   " << value * 1000.0 << " mV" << std::endl;
	std::cout << "current = " << current << " mA" << std::endl;
	std::cout << "current (last_value) = " << voltage_to_current(dac.code_to_voltage(dac.get_last_value())) << std::endl;
}

int main (void){
	setup();
	auto loop_start = std::chrono::high_resolution_clock::now();
	loop();
	double loop_duration = time_since(loop_start);
	for (;;){
//		for(int j = 0; j < every / loop_duration ; j++){
			loop();
//		}
//		routine();
	}
	return 0 ;
}

double current_to_voltage(double current){
	return - current * 0.05246298729 * 50.0 / 53.9; 
}

double voltage_to_current(double voltage){
	return voltage / current_to_voltage(1);
}
