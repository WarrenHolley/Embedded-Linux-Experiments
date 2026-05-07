obj-m += wgh_gpio_experiment.o

all: driver overlay

driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
# Device Tree Overlay - Needs research.
overlay: wgh_gpio_led_overlay.dts
	dtc -I dts -O dtb wgh_gpio_led_overlay.dts -o wgh_gpio_led_overlay.dtdo

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) wgh_gpio_led_overlay.dtbo
