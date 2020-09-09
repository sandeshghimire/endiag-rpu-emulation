KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

obj-m:= rpu_emulator.o

default:
	make -C $(KDIR) M=$(PWD) modules

clean:
	$(RM) *.o  *.mod.c Module.symvers modules.order *.cmd  *.rpu_emulator.o.cmd *.mod  .rpu_emulator.ko.cmd  .rpu_emulator.mod.cmd   .rpu_emulator.mod.o.cmd .rpu_emulator.o.cmd  rpu_emulator.ko 

install:
	cp *.ko ../../install 