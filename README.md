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


## Demo


## References








