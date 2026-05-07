#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
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

// pwm-device -> Just needs a device Struct.
// struct pwm_device *pwm_get(struct device *dev, const char *con_id)
// Device Struct loaded from Platform Module loading procedure?
// Might be able to get the Device handle directly, as the Platform Module nonsense appears to be mostly for I/O sub-handling.

// --------------------------------------------------
uint32_t pwm0_strength = 33; // Percent. Kernel uses nanosections.
uint32_t pwm1_strength = 66; // Just calculate it in-script rather than have a bunch of zeroes.

// Just do PWM0 for now.
struct pwm_device *pwm0_dev = NULL;
//struct pwm_device *pwm1_dev = NULL;

// --------------------------------------------------

static void load_pwm_module(void)
{ 
  // Given the pwm0 struct loaded, just configure & enable here.
  
  uint32_t pwm0_on_time = pwm0_strength * 1000000;
  uint32_t pwm1_on_time = pwm1_strength * 1000000; // Not used.
  
  pwm_config(pwm0_dev, pwm0_on_time, 1000000000);
  // pwm_config(pwm1, pwm1_on_time, 1000000000);
  
  pwm_enable(pwm0_dev); //WGH:Research - Check if the config can be changed after enabling.
  // pwm_enable(pwm1);
}

static void unload_pwm_module(void){
  pwm_disable(pwm0_dev);
  // pwm_disable(pwm1);
}

// ------ Platform Driver Nonsense to get the pwm_device --------------------------------

static int dev_probe(struct platform_device *pdev)
{ 
  struct device *dev = &pdev->dev; // Extract the generic 'device' struct.
  
  // Don't need to handle any of the File-ops or misc-device registration, so just skip the my_dev_data struct used in the GPIO experiment.
  
  // And get the pwm_device from the generic device, 
  pwm0_dev = pwm_get( dev, "pwm0" ); //Not sure how to expand this to both, unless using a different fragment?

  return 0; //Return Success
}

static void dev_remove(struct platform_device *pdev)
{
  // Just need to release the PWM device.
  pwm_put( pwm0_dev );
}



static const struct of_device_id wgh_device_ids[] = { // Configure overlay compatible.
  { .compatible = "wgh,pwm_experiment"}, //Not sure how it finds this without a name, unless it's assumed as the KO file's in the same folder.
  {} 
  //Empty set marks the end of the list.
};
MODULE_DEVICE_TABLE( of, wgh_device_ids ); // Load the overlay.

static struct platform_driver wgh_pwm_driver = {
  .probe  = dev_probe,
  .remove = dev_remove,
  .driver = {
    .name = "wgh_pwm_driver",
    .of_match_table = wgh_device_ids,
  }
};

// ------ Module Point --------------------------------
// Apparently need to only run the following code once, so do the following?
// Preprocessor? I can't recall the name.
// Save the __init and __exit points, otherwise load the module platform driver.
#if 1
static int __init wgh_init_module(void) {
  printk("Loading PWM-test Module...\n");
  load_pwm_module();
  return 0;
}
static void __exit wgh_exit_module(void) {
  printk("Exiting PWM-test Module...\n");
  unload_pwm_module();
}
module_init( wgh_init_module );
module_exit( wgh_exit_module );
#else
module_platform_driver( wgh_pwm_driver ); // Not sure why this has to be filtered out.
#endif
	 

