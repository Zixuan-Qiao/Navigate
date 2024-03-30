# Navigate
A simple maze-solving game on Beaglebone Black. 

## Introduction
In this game, the user will walk through an automatically generated maze with a joystick. 
A piezo will beep to notify the user that they have hit a wall. 

The functionality is implemented by two main modules:
### Linux kernel-space driver

### User-space application
The user-space application is in charge of the generation and verification of the maze. 
It also interacts with the Linux Industry I/O subsystem and the piezo driver to control the peripheral devices. 
Its workflow is illustrated below.

## Schematic

## Demo

## References







