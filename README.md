# Navigate
A simple maze-solving game on Beaglebone Black. 

## Introduction
In this game, the user will walk through an automatically generated maze with a joystick. 
A piezo will beep to notify the user if they have hit a wall. 

The functionality is implemented by two main modules:

### Linux kernel-space driver
A GPIO driver is implemented to control the piezos. It can manage multiple piezos simultaneously and the number can be specified at load time. 

It provides a write function that allows the caller to determine the frequency and length of the beep. 

Locking mechanisms are implemented to make it safe in symmetric multiprocessing (SMP) and multi-user environments. 

### User-space application
The workflow of the user-space application starts with generating a random map. The size and obstacle ratio are easily adjustable. The map is then varified with the Hadlock's algorithm to make sure that it is solvable. 

The program interacts with the Linux Industry I/O subsystem to obtain analog input from the joystick and determine the next move accordingly. 

If the next move and the map indicates that the user has hit a wall, it will issue a system call to the piezo driver to send vocal notifications to the user.  

## Schematic
**Joystick**

<img width="500" alt="schematic" src="https://github.com/Zixuan-Qiao/Navigate/assets/102449059/530bac0f-32ab-4d01-abe4-b10844a6202c">

**Piezo**

<img width="350" alt="2" src="https://github.com/Zixuan-Qiao/Navigate/assets/102449059/ad845501-ace8-4b36-9305-53ba1543a021">

## Demo


## References
1. https://lwn.net/Kernel/LDD3/
2. https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Tone.cpp
3. https://askubuntu.com/questions/882433/enable-adc-input-on-beaglebone-with-4-x-kernel
4. https://www.kernel.org/doc/html/v5.10/driver-api/iio/index.html
5. https://wiki.analog.com/software/linux/docs/iio/iio
6. https://elinux.org/EBC_Exercise_10a_Analog_In
7. https://www.linux.com/training-tutorials/how-get-analog-input-beaglebone-black/
8. https://vadl.github.io/beagleboneblack/2016/07/29/setting-up-bbb-gpio
9. https://www.derekmolloy.ie/kernel-gpio-programming-buttons-and-leds/
