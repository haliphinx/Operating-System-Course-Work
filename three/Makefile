ifneq ($(KERNELRELEASE),)    
    
obj-m := hxdriver.o        
else
PWD :=$(shell pwd)
KVER :=$(shell uname -r)
KDIR :=/lib/modules/$(KVER)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
#	rm -f *.cmd *.o *.mod *.ko
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
#	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
