#include <linux/module.h>
#include <linux/init.h>

#include <linux/pwm.h> // PWM Interface.

// Set up the Kernel Module Data.
MODULE_AUTHOR("WGHolley");
MODULE_DESCRIPTION("Simple Kernel-Module (not driver?) for PWM LEDs. No Userspace control");
MODULE_LICENSE("GPL");

// Note: The previous fancy systems have been deprecated, for now.
// Just running this as a baseline kernel module.
// I'm reducing the complexity, as for the life of me, I can't get the Device Overlay to work with PWM.
// May try to re-introduce them once I actually understand the DT.

// --------------------------------------------------

struct pwm_device *pwm0; // Use both on the board. Pins 32+33, or GPIO 12+13.
struct pwm_device *pwm1; 

pwm0_strength = 33; // Percent. Kernel uses nanosections.
pwm1_strength = 66; // Just calculate it in-script rather than have a bunch of zeroes.
// --------------------------------------------------

static void load_pwm_module(){
  // Just load things up. Don't faff about checking statuses.
  pwm0 = pwm_request(0, "low_power_led");
  pwm1 = pwm_request(1, "mid_power_led");
  
  uint32_t pwm0_on_time = pwm0_strength * 1000000;
  uint32_t pwm1_on_time = pwm1_strength * 1000000;
  
  pwm_config(pwm0, pwm0_on_time, 1000000000);
  pwm_config(pwm1, pwm1_on_time, 1000000000);
  
  pwm_enable(pwm0); //WGH:Research - Check if the config can be changed after enabling.
  pwm_enable(pwm1);
}

static void unload_pwm_module(){
  pwm_disable(pwm0);
  pwm_disable(pwm1);
  
  pwm_free(pwm0);
  pwm_free(pwm1);
}

// --------------------------------------------------


static int __init init_module(void) {
  printk("Loading PWM-test Module...\n");
  load_pwm_module();
  return 0;
}

static void __exit exit_module() {
  printk("Exiting PWM-test Module...\n");
  unload_pwm_module();
}

module_init(init_module);
module_exit(exit_module);
	
	 

