## PWM Test.
A minimalist kernel-module (not driver) test to try to get PWM working.  
Experimenting with getting PWM working on the Device Tree has so far been for naught.  

### Install & Run:
$> make
$> sudo insmod wgh_pwm_module.ko

### Hardware Setup:
Hook up LEDs to Pins 32+33, PWM_0 and PWM_1 respectively.  
Pin 39 (Gnd) - Ground  

Resistor - 100 Ohm  
Red LED - ~1.8 FVD  
(Est 15mA current at 100%)  
As PWM driver running at 33 and 66% respectively, should be ~5mA and ~10mA.  

