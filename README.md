# Fusion egg - An audioreactive festival totem

<img src="media/grafiken/fusion_bilder/5.jpg" width=50% height=50%>

## Overview

This repository contains software and cad-files for an audioreactive festival totem. This totem is used to find ones crew at a music festival. It listens to the music, detects the beat and translates into a visual animation.

## Structure

## Build
To build your own totem you need the following tools:

- 3D-Printer (z-height min 250mm , Nozzle 0.8mm)
- Laser cutter to cut the internal skeleton of the egg
- Soldering iron

The following parts are needed:

- ESP32 microcontroller board
- Microphone MAX4466
- Accelerometer ADXL345
- WS2812 LED-Stripes (144 leds per meter), for the three stripe egg you need a little bit more than 1 meter. The six stripe egg requires more than 2 meters.
- USB-Powerbank
- Micro USB-Cabel
- Wago Connectors 221-415
- Cables
- Heat-shrink tubing
- Aluminium Tube (OD 20mm, ID 17mm, 2m long)
- 6 M4x20mm Screws
- 12 M4x15mm Screws
- 18 Heat-set inserts M4x6x6
- 3mm Plywood
- Two colors of PLA (light and dark color)
- Cable-ties

### 3D Printing
For the three stripe version you have to print the following files:
- cad_3stripes/vase.stl (Vase-Mode)
- cad_3stripes/boden.stl 
- cad_3stripes/deckel.stl
- cad_3stripes/klaue.stl

For the six stripe version the folling parts you need the files from the cad_6stripes folder.

### Laser-Cutting
cad_3stripes/lasercutter.pdf contains all parts you need to cut for the 3strip version. 

Assemble the parts with wood glue inside the vase printed part, as showed in this explosion diagramm:
<img src="media/grafiken/cad/explosion.PNG" width=75% height=75%>

### Electronics
Connect all electronics as showed here:
<img src="media/fritzing/fritzing_bb.png" width=100% height=100%>

### Software
Build and flash the sofware inside the firmware folder with PlatformIO. If you need to change the pin assignment for the leds or the microphone, you can find the definitions inside firmware/include/zauberstab.h

## License
All software is licensed under GPLv2. Hardware is licensed under CC BY-NC-SA. Contact us for commercial use.

## Contact
You can contact us at tom@binary-kitchen.de and buddhabrot@binary-kitchen.de
