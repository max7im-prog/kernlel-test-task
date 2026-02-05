MODULE_NAME = hello
PWD := $(CURDIR)

obj-m += $(MODULE_NAME).o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD)  clean

load: all
	sudo mkdir /var/tmp/test_module
	sudo insmod ./$(MODULE_NAME).ko

unload:
	sudo rmmod $(MODULE_NAME)

.PHONY: all clean load unload
