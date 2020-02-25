#
## legOS simulator
## Makefile.common - common make rules
## (c) 1998 by Markus L. Noga <noga@inrialpes.fr>    
##     1999 Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
##

TARGET=hello

#location
HERE=examples

# kernel sources & objects
KSOURCES=$(SIM_ROOT)LINUX/kmain.c \
        $(SIM_ROOT)LINUX/direct-button.c \
        $(SIM_ROOT)LINUX/tm.c \
	$(SIM_ROOT)LINUX/direct-sensor.c \
        $(SIM_ROOT)LINUX/direct-motor.c \
        $(SIM_ROOT)LINUX/direct-ir.c \
	$(SIM_ROOT)LINUX/conio.c \
        $(SIM_ROOT)LINUX/lcd.c

KOBJECTS=$(KSOURCES:.c=.o)
EXECS = hello light-sensor rotation-sensor simple-rover stear tm-and-ir
      
# user sources
USOURCES=$(TARGET).cpp

# linker command file.
# necessary because h8300 standard linkage stubbornly places
# read-only data segment in front of text segment. 
# took a while to figure that out.
LDFLAGS=-L$(SIM_ROOT)lib -L/usr/lib

# link all libraries. they consist of individual files per function,
# so nothing unnecessary will be linked.
LIBS=

all: $(EXECS)

include ../../Makefile.common

VPATH=$(SIM_ROOT)examples

clean:
	rm -rf *.o *.map *.coff *.srec *.dis* *~ *.bak *.tgz *.s tags demo/*.o demo/*.map demo/*.coff demo/*.srec demo/*.dis* $(EXECS)

realclean: clean
	(cd lib; make clean)
	rm -f doc/html/*.html

hello: hello.o
	$(LD) hello.o $(LDFLAGS) $(KOBJECTS) -o hello $(TLIBS_DIR) $(TLIBS) $(LIBS)

light-sensor: light-sensor.o
	$(LD) light-sensor.o $(LDFLAGS) $(KOBJECTS) -o light-sensor $(TLIBS_DIR) $(TLIBS) $(LIBS)

simple-rover: simple-rover.o
	$(LD) simple-rover.o $(LDFLAGS) $(KOBJECTS) -o simple-rover $(TLIBS_DIR) $(TLIBS) $(LIBS)

stear: stear.o
	$(LD) stear.o $(LDFLAGS) $(KOBJECTS) -o stear $(TLIBS_DIR) $(TLIBS) $(LIBS)

rotation-sensor: rotation-sensor.o
	$(LD) rotation-sensor.o $(LDFLAGS) $(KOBJECTS) -o rotation-sensor $(TLIBS_DIR) $(TLIBS) $(LIBS)

tm-and-ir: tm-and-ir.o
	$(LD) tm-and-ir.o $(LDFLAGS) $(KOBJECTS) -o tm-and-ir $(TLIBS_DIR) $(TLIBS) $(LIBS)
