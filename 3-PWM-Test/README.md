## PWM Test.
A minimalist kernel-module (maybe driver) test to try to get PWM working.  
Experimenting with getting PWM working on the Device Tree has so far been for naught.  

### Install & Run:
$> make  
$> sudo dtoverlay wgh_pwm_overlay.dtbo # Note - Logs show memory leak on removal? Diagnose.  
$> sudo insmod wgh_pwm_module.ko # Still putzed. Not sure why.  

### Hardware Setup:
Pin 18 - Positive PWM source - PWM_0  
Pin 39 (Gnd) - Ground  

Resistor - 100 Ohm  
Red LED - ~1.8 FVD  
(Est 15mA current at 100%)  
As PWM driver (will be) running at 33 and 66% respectively, should be ~5mA and ~10mA.  

