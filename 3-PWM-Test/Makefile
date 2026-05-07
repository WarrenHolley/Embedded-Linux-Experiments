obj-m += wgh_pwm_module.o

all: driver overlay

driver:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
# Device Tree Overlay - Needs research.
overlay: wgh_pwm_overlay.dts
	dtc -I dts -O dtb wgh_pwm_overlay.dts -o wgh_pwm_overlay.dtbo

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	$(RM) wgh_pwm_overlay.dtbo
