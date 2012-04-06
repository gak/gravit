# Makefile stub for MinGW gcc
# this makefile just calls ../Makefile.MinGW
#
#
# On 32bit windows, run "make 32bit" 
# On 64bit windows, run "make 64bit" 
#


all: show_options

show_options:
	@echo 
	@echo make 32bit    to build using 32-bit libraries
	@echo 
	@echo make 64bit    to build using 64-bit libraries
	@echo 
	@echo make install32 or make install64
	@echo "             to prepare for building the setup program"
	@echo 
	@exit 1


32bit:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=32

64bit:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=64


clean:
	$(MAKE) -C .. -f Makefile.MinGW clean



#install:
#	$(MAKE) -C .. -f Makefile.MinGW install

install32:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=32 install

install64:
	$(MAKE) -C .. -f Makefile.MinGW ARCH=64 install



.phony: show_options
.phony: 32bit
.phony: 64bit
.phony: clean
.phony: install