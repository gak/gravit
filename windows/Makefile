# Makefile stub for MinGW gcc
# this makefile just calls ../Makefile.MinGW
#
#
# On 32bit windows, run "make 32bit" 
# On 64bit windows, run "make 64bit" 
#


all:
	$(MAKE) -C .. -f Makefile.MinGW


32bit:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=32

64bit:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=64


clean:
	$(MAKE) -C .. -f Makefile.MinGW clean



install:
	$(MAKE) -C .. -f Makefile.MinGW install

install32:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=32 install

install64:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=64 install



.phony: 32bit
.phony: 64bit
.phony: clean
.phony: install