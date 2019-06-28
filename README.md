# LX-1-teensy
This project aims to receive Carbon Video System's StormBreaker protocol and interface with an ODrive system accordingly.

## StormBreaker
Relevant data is passed to the Teensy from a Raspberry Pi.  This data is parsed and then used to control an ODrive motor system.

## Connections
USB-FTDI based UART connection to a Raspberry Pi 3. \
UART connection to an ODrive system.

# Project Management
## Changelog
6/20/19: Added pan and tilt functionality \
6/26/19: Refactored preprocessor directives \
6/27/19: Added licensing and documentation.

## TODO
Improve pan and tilt code. \
Add iris, zoom, focus functionality. \
Handle mode switch cases. \

Handle override cases. \
