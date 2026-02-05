MODULE_NAME=hello

obj-m += module/$(MODULE_NAME).o   
   
PWD := $(CURDIR)  
   
all: build

build:  
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
  
clean: 
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: build
	sudo insmod module/$(MODULE_NAME).ko

unload: 
	sudo rmmod $(MODULE_NAME)
