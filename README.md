# arduino_train
Arduino sketches for controlling my specific train layout utilizing an ESP32.

Most parts of this repository are pre-studies that I am using for desktop testing only. These parts will be deleted soon.

The first part, that will be used for controlling a real train, is in the directory ReverseLoop. As its name suggests, that sketch will be controlling one train through a reversing loop back and forth. I.e. a laout as this:

-----------------------\
          \             \
           \             |
            \           /
             \         /
              \-------/
              
The train control is targeting a yet non-existing LGB and/or Piko garden train layout with a relatively long stretch of track with a reversing loop in each end and sidings along the stretch as well as in one of the reversing loops. But that is not until the first reversing loop is fully functional.

The hardware consist of:
  - ESP32 DoIT module (serving the train control and a WEB page turning a phone into the UI)
  - SD card reader to store the HTML files, icons etc.
  - 20 volt power supply to drive the train
  - 5 volt power supply for the control system
  - Relay module for reversing the polarity to make the train reverse
  - Later on more relays to control power to sidings
  - Reed switches and a magnet on the train to sense the position of the train
  - Transistor power amplifier(s) to control the speed of the trains via ESP32 DACs
  - Micro servos to control turnouts
  
