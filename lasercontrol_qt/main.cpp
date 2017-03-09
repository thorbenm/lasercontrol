#include "mainwindow.h"
#include <QApplication>


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
}


int main(int argc, char *argv[])
{
    setup();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

bool voltage_out_of_range(double voltage){
        if(max_voltage < voltage){
//                std::cerr << "Voltage / Current out of Range\nProgramme will be closed \n";
                return 1;
        }
        else if(voltage < min_voltage){
//                std::cerr << "Voltage / Current out of Range\nProgramme will be closed \n";
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


bool transmit(uint16_t code, uint8_t device, unsigned int channel){
        uint8_t code1 = code >> 8;
        uint8_t code2 = code & 0xFF;
        wiringPiSPIDataRW (channel, (unsigned char*)&device, sizeof(device));
        wiringPiSPIDataRW (channel, (unsigned char*)&code1, sizeof(code1));
        wiringPiSPIDataRW (channel, (unsigned char*)&code2, sizeof(code2));
        digitalWrite (LDAC,LOW);
//      delayMicroseconds(1);
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

double current_to_voltage(double current){
        return - current * 0.05246298729;
}

double voltage_to_current(double voltage){
        return voltage / current_to_voltage(1);
}
