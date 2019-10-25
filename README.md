<<<<<<< HEAD
# arduino_train
Arduino sketches for controlling my specific train layout
=======
# Arduino code for garden train control
Arduino sketches for controlling my specific train layout utilizing an ESP32.

Most parts of this repository are pre-studies that I am using for desktop testing only. These parts will be deleted soon.

The first part, that will be used for controlling a real train, is in the directory ReverseLoop. As its name suggests, that sketch will be controlling one train through a reversing loop back and forth.
              
The train control is targeting a yet non-existing LGB and/or Piko garden train layout with a relatively long stretch of track with a reversing loop in each end and sidings along the stretch as well as in one of the reversing loops. But that is not until the first reversing loop is fully functional.

## Hardware

The hardware consist of:
  - ESP32 DoIT module (serving the train control and a WEB page turning a phone into the UI)
  - SD card reader to store the HTML files, icons etc.
  - 20 volt power supply to drive the train
  - 5 volt power supply for the control system and servos
  - Relay module for reversing the polarity to make the train reverse
  - Later on more relays to control power to sidings
  - Reed switches and a magnet on the train to sense the position of the train
  - Transistor power amplifier(s) to control the speed of the trains via ESP32 DACs
  - Micro servos to control turnouts
  - Possibly a PCA9685 based Arduino module to add more PWM ports for controlling servos
  - Possibly an MCP23017 based Arduino module to add more digital I/O ports for reed switches
  - Possibly I2C DAC and ADC modules if there is a need for more than two power amplifiers

See more at http://lisby.dk/wordpress/?p=2106 (so far only in Danish).
>>>>>>> 8ea88b605b6db311ffbe342b107a8a424156c0f1
