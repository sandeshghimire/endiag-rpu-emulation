KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

obj-m:= rpu_emulator.o

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	$(RM) *.o  *.mod.c Module.symvers modules.order

install:
	cp *.ko ../../install 