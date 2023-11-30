# kbuild part of makefile
obj-m := hello1.o hello2.o

ccflags-y += -I$(src)/inc

# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean