obj-m += wgh_gpio_experiment.o

all: driver wgh_gpio_led_overlay.dtbo

driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
# Device Tree Overlay - Needs research.
wgh_gpio_led_overlay.dtbo: wgh_gpio_led_overlay.dts
	dtc -I dts -O dtb $< -o $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) my_overlay.dtbo
