# CelClock_2021
 These are CelClock 2021 source files.
 
 The program was written and compiled for the STM32G071CBT6.
 
 Download the schematic and a 3D model files (both are PDFs) to see the electrical connections, and the PCB layout.

## The following libraries are included:
-WS2812B LEDs library - ws_lib.c, ws_lib.h

-16-seg LED display driver - segment_dsp.c, segment_dsp.h

-RTC configuration - clock.c, clock.h

-Menu and UI settings - clk_menu.c, clk_menu.h


### About the project:
The primary objective is to display the current time and date on the PCB front equipped with a series of WS2812B intelligent RGB LEDs. The STM32G071CBT6 microcontroller is in charge of the system control. It also consists of an embedded RTC clock that determines the current time to be displayed. The hardware design allows for battery support of the Battery Backup Domain registers via a supercapacitor and writing data to an external 2kB EEPROM memory.
