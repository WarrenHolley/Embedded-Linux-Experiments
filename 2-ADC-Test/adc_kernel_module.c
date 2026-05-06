#include <linux/module.h>
#include <linux/init.h>

#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>

// Set up the Kernel Module Data.
MODULE_AUTHOR("WGHolley");
MODULE_DESCRIPTION("Simple ADC experiment for RPi4.");
MODULE_LICENSE("GPL"); // Need to look into what this does beyond warn.

// --------------------------------------------------

// Build a struct of all compatible devices - compatible strings must match.
static const struct of_device_id wgh_device_ids[] = {
  { .compatible = "wgh,adc_test"},
  {} 
  //Empty set marks the end of the list.
};
MODULE_DEVICE_TABLE( of, wgh_device_ids );

//Set up a struct to track each device.
struct my_dev_data {
  struct pwm_device *pwm; //LED Pin, dimmed over PWM.
  struct gpio_desc  *adc; //ADC Pin, toggled by potentiometer.
  struct miscdevice mdev;
};

// Keep a count of the number of devices attached. Global Var.
static int device_count = 0;

// --------------------------------------------------

// WGH: File-IO plan was to have a readable & writable single-file.
// Not sure if this is practical, or useful.
// Need to play around with writable files beyong single-char on/off used in the blinkenlight.
// Assume none of the following work.

// Userspace I/O interface.
static inline struct my_dev_data *to_my_dev_data(struct file *filp)
{ // File Interface? (WGH:Research)
  struct miscdevice *m = filp->private_data;        //Filepointer - Acts as a File Handle.
  return container_of(m, struct my_dev_data, mdev); //container_of is black magic, as far as I can tell.
}

// Write interface. String input, 0-100.
// Controls the PWM duty cycle.
static ssize_t pwm_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{
  // WGH:Research - Goddamn file-streams.
  // AFAI Understand: When fed data, reads anything new, ignores the rest.
  // I have no idea if this is anywhere near safe.
  struct my_dev_data *priv = to_my_dev_data(filp);
  char text[16]; 
  int not_copied, delta, to_copy = (len + *off) < sizeof(text) ? len : (sizeof(text) - *off);

  // WGH:Research. Just returns if given larger datablock?
  if (*off >= sizeof(text))
    return 0;

  not_copied = copy_from_user(&text[*off], user_buf, to_copy);
  delta = to_copy - not_copied;

  if (text[0] == '0') gpiod_set_value(priv->led, 0);
  if (text[0] == '1') gpiod_set_value(priv->led, 1);

  *off += delta;
  return delta;
}

static ssize_t adc_read (struct file *filp, char __user *user_buf, size_t len, loff_t *off)
{

}


static struct file_operations fops = {
  .write = pwm_write
  .read  = adc_read
};


// --------------------------------------------------
// Device Probe Setup & Deletion.
static int dev_probe(struct platform_device *pdev)
{ 
  struct device *dev = &pdev->dev;
  
  struct my_dev_data *device_struct; // Set up a private struct for the devices.
  device_struct = devm_kzalloc(dev, sizeof(struct my_dev_data), GFP_KERNEL);
  // Managed? devm_kzalloc used instead of kzalloc as to not need to free
  //  the memory after error, just need to delete the device bindings.

  // Check for malloc error.
  if ( device_struct == 0 ) {
    dev_err(dev, "Device kzalloc failure\n");
    return -ENOMEM;
  }

  // Get the GPIO Descriptor for the LED.
  device_struct->pwm = pwm_get( dev, con_id ); //NOTE: No clue what con_id currently is.
  // Check for error.
  if ( IS_ERR(device_struct->pwm) ) {
    dev_err(dev, "Could not read in pwm-gpio property!\n");
    return -1; // Doesn't look like need to delete bindings for the devm_kzalloc, as will fall out of scope? (WGH:Research)
  }
  
  // Putzed from here, as there's no onboard ADC.
  
  // Get the GPIO Descriptor for the LED.
  device_struct->adc = gpiod_get( dev, "adc", GPIOD_OUT_LOW );
  // Check for error.
  if ( IS_ERR(device_struct->adc) ) {
    dev_err(dev, "Could not read in adc-gpio property!\n");
    // Will need to de-bind the PWM hook, as it'll be binded at this point.
    gpiod_put(device_struct->pwm);

    return -1;
  }
  

  //Manage the internal miscdevice struct.
  device_struct->mdev.fops  = &fops;              // file_operations - internal structure.
  device_struct->mdev.minor = MISC_DYNAMIC_MINOR; // Minor Device ID - Autogenerate.
  
  char name[32];
  sprintf(name, "wgh_adc_%d", device_count++); 
  // Should take only 19 B w/ terminator, so can instantiate 10^13 objects before overflow... Pretty sure Linux'll explode before then.
  device_struct->mdev.name = name; // Should be "wgh_blinkenlight_0"
s
  // Register the new miscellaneos device. Creates the device file. (WGH:Research)
  int status = misc_register(&device_struct->mdev);
  // Check for failure. (Nonzero)
  if (status) {
    dev_err(dev, "Failed to register the LED\n");
    gpiod_put(device_struct->led); // Dispose of the GPIO Descriptor.
    return status;
  }
  
  // Bind new structure to Platform Device (WGH:Research)
  platform_set_drvdata(pdev, device_struct);
  // Don't check for failure? Assumed by the misc_register success? (WGH:Research)
  // I've seen multiple different Docs and Tutorials doing these steps in odd ways.
  
  return 0; //Return Success
}

static void my_dev_remove(struct platform_device *pdev)
{
  //From the Platform Device Driver Data struct, pull out the device struct.
  struct my_dev_data *priv = platform_get_drvdata(pdev);

  //Decriment and deallocate everything. Priv set up via devm_kzalloc, so no need to free.
  device_count--;
  misc_deregister(&priv->mdev);
  gpiod_set_value(priv->led, 0);
  gpiod_put(priv->led);
}

// Build a struct with the required prove & remove functions.
static struct platform_driver wgh_blinkenlight_driver = {
  .probe  = dev_probe,
  .remove = dev_remove,
  .driver = {
    .name = "wgh_blinkenlight_driver",
    .of_match_table = wgh_device_ids,
  }
};

// Register the driver within the OS.
module_platform_driver( wgh_blinkenlight_driver );


