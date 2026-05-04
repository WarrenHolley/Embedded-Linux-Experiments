#!/bin/bash
# Simple script to run though the compile & install process.
# Flags:
# --install   - Compile & setup the kernel module
# --uninstall - remove the kernel module.

if [ "$EUID" -ne 0 ]; then
  echo "Needs to be run as root to create the device file."
  exit
fi

if   [ "$1" == "--install" ]; then
  echo "Installing..."
  make 
  dtoverlay wgh_gpio_led_overlay.dtbo # Note: Running more than once adds multiple overlays. Doesn't mess with the blinking though WGH:Research
  insmod wgh_gpio_experiment.ko

  # Should probably check for errors.
  echo "Installed!"
  echo "Run wgh_blinkenlight_toggle or write 1's and 0's manually to dev/wgh_blinkenlight_* to experiment!"
  
  
elif [ "$1" == "--uninstall" ]; then
  echo "Uninstalling..."
  dtoverlay -r wgh_gpio_led_overlay
  rmmod wgh_gpio_experiment.ko
  echo "Uninstalled!"
  

else
  echo "Must be given an arg: --install or --uninstall."
  echo " Just for safety."
  echo " And because auto-detection would make this 10x as complicated."
  echo " -WGH"
fi
