#!/usr/bin/python
 
import spidev
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
 
LDAC = 25
GPIO.setup(LDAC,GPIO.OUT)
GPIO.output(LDAC,GPIO.HIGH)

spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 7629
   
def transmit(device, code):
    msb = code >> 8
    lsb = code & 0xFF
    spi.xfer([device, msb, lsb])
    GPIO.output(LDAC,GPIO.LOW)
  #  time.sleep(0.000001)
    GPIO.output(LDAC,GPIO.HIGH)
                    
while True:
    transmit(3,0x0000)
    time.sleep(0.5)
    transmit(3,0xFFFF)
    time.sleep(0.5)
