#include <linux/module.h>
#include <linux/init.h>

#include <linux/pwm.h> // PWM Interface.

// Set up the Kernel Module Data.
MODULE_AUTHOR("WGHolley");
MODULE_DESCRIPTION("Simple Kernel-Module (not driver?) for PWM LEDs. No Userspace control");
MODULE_LICENSE("GPL");


// Note: Version 1 - The fancy DT Overlay and handling systems have were deprecated.
//  The plan was to run this as a baseline kernel module, as there -were- simple interfaces in pwm.h that 
//   allowed simple access to the PWM pins.
//  That plan was a failure, as apparently the pwm_request system was deprecated, in favor of using the Device Tree.
//  Which, at time of writing, was giving me hell in trying to make work.

// --------------------------------------------------

uint32_t pwm0_strength = 33; // Percent. Kernel uses nanosections.
uint32_t pwm1_strength = 66; // Just calculate it in-script rather than have a bunch of zeroes.

// pwm_get deprecated. No longer included in the pwm.h library.
// struct pwm_device *pwm0 = NULL; // Use both on the board. Pins 32+33, or GPIO 12+13.
// struct pwm_device *pwm1 = NULL; 

// Apparently needs a generic device to request it, which is to pull from the Device Tree.
// ... which is becoming a system that I'm starting to despise it's opaqueness.
struct device *pwm0_d = NULL;
struct device *pwm1_d = NULL;

// --------------------------------------------------
// ------ From here down it's all broken ------------
// --------------------------------------------------


// --------------------------------------------------

static void load_pwm_module(void){
  // All broken. pwm_request was deprecated in favor of pwm_get, but that requires
  //  a Device handle, which needs to be pulled from the Device Tree?
  
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

static void unload_pwm_module(void){
  pwm_disable(pwm0);
  pwm_disable(pwm1);
  
  pwm_free(pwm0);
  pwm_free(pwm1);
}

// --------------------------------------------------


static int __init init_module_Test(void) {
  printk("Loading PWM-test Module...\n");
  load_pwm_module();
  return 0;
}

static void __exit exit_module_Test(void) {
  printk("Exiting PWM-test Module...\n");
  unload_pwm_module();
}

module_init(init_module_Test);
module_exit(exit_module_Test);
	
	 

