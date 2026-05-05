# Experiment with Embedded Linux GPIO.
An experiment to get a blinking LED running on a Rasperry Pi 4B GPIO Pin via a kernel module.


### Install (RPi 4B):
$> sudo bash wgh_blinkenlight_setup.sh --install  
Will automatically compile the code, load the device overlay, then load the firmware module.  
Debian's got everything needed already installed. (Need to figure out dependencies)

### Uninstall:
$> sudo bash wgh_blinkenlight_setup.sh --uninstall  
Uninstalls the firmware module, unloads the device overlay.

### Hardware Setup:
Pin 40 (GPIO 21) - Positive signal - 3.3V  
Pin 39 (Gnd) - Ground  
Resistor - 100 Ohm  
Red LED - ~1.8 FVD  
(Est 15mA current)  

### Run:
$> sudo bash wgh_blinkenlight_setup.sh --install  
$> sudo bash wgh_blinkenlight_toggle.sh

The sudo calls are required to create, then write to, the Character Device File.


### Notes:
This was just to experiment with the Linux Kernel Module.  

I've worked with C on Linux.  
I've worked with C on Embedded Systems.  
I've worked with Linux on Embedded Systems.  
I have yet to work with C, on Linux, on Embedded Systems.  
And I found that hardware access is in kernel-space, not user-space.  
And boy howdy are they -entirely- different beasts.
