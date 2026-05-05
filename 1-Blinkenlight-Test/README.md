## Blinkenlight - The End Product of a Weekend of Pain
An experiment to get a blinking LED running on a Rasperry Pi 4B GPIO Pin via a kernel module.  

Page after page of documentation, book references, youtube videos, StackOverflow threads, all culminated in this... mess.  
This was published first, as it was the first product I was happy with, and could be shown to work without sharing a terminal.  

It blinked an LED, it had a userspace interface, and despite my rookie efforts, didn't explode after 30s.  
I roughtly understand each component, but the whole is currently beyond me.  
I'll return to this once I understand how these components interact.

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
