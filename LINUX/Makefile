##
## legOS simulator
## Makefile - allows you to keep the upper hand
## (c) 1998 by Markus L. Noga <noga@inrialpes.fr>    
##     1999 Thomas Roeblitz <roeblitz@informatik.hu-berlin.de>
##

# our target
TARGET=kmain.o

#location
HERE=src

# kernel sources & objects
KSOURCES=kmain.c direct-button.c tm.c direct-button.c \
         direct-sensor.c direct-motor.c conio.c \
	 lcd.c direct-ir.c

## mm.c systime.c tm.c semaphore.c conio.c lcd.c \
##         direct-ir.c direct-motor.c direct-sensor.c direct-sound.c
         
KOBJECTS=$(KSOURCES:.c=.o)
      
# user sources
USOURCES=$(TARGET).cpp

# linker command file.
# necessary because h8300 standard linkage stubbornly places
# read-only data segment in front of text segment. 
# took a while to figure that out.
#LDFLAGS=-T sim_i386linux.x -relax -Llib

# link all libraries. they consist of individual files per function,
# so nothing unnecessary will be linked.
#LIBS= -lc -lmint -lc++

# do we need to pad the srec file with zeroes?
#NEED_ZERO_PADDING=--pad-to 0xC000


##
## older options
##

# how to disassemble
ODFLAGS = --disassemble-all --no-show-raw-insn -m h8300

# which firmware version to act upon
FIRMVERS=firm0309


##
## no user servicable parts below
##

all: $(KOBJECTS)

include ../Makefile.common

VPATH=$(SIM_ROOT)src

# doc/html subdirectory
html:
	doxygen Doxyfile

# libraries subdirectory
libs:
	(cd lib; make)

depend:
	makedepend ../src/*.c -Y../include

tag:
	ctags *.c include/*.h include/*/*.h $(USOURCES)

clean:
	rm -rf *.o *.map *.coff *.srec *.dis* *~ *.bak *.tgz *.s tags demo/*.o demo/*.map demo/*.coff demo/*.srec demo/*.dis*

realclean: clean
	(cd lib; make clean)
	rm -f doc/html/*.html
		
# how to build coff kernel / application image
%.coff: %.o $(KOBJECTS) libs
	$(LD) $(LDFLAGS) $(KOBJECTS) $*.o $(LIBS) -o $*.coff --oformat coff-h8300
	chmod a-x $*.coff

###
### generic rules
###

# how to compile C source
%.o: %.c
	$(CC) $(CFLAGS) -g -c $(SIM_ROOT)src/$*.c -o $*.o

# how to make a srec file of the appropriate length
%.srec: %.coff	
	$(OBJCOPY) -I coff-h8300 -O srec $(NEED_ZERO_PADDING) $*.coff $*.srec
	chmod a-x $*.srec

# how to merge map files with symbols
%.dis2: %.map %.dis
	util/merge-map $*.map $*.dis > $*.dis2
        
# how to make map files
%.map: %.coff
	$(NM) $*.coff | sort -u > $*.map

# how to disassemble new firmware files
%.dis: %.coff
	$(OBJDUMP) $(ODFLAGS) $*.coff > $*.dis
	
# how to disassemble new firmware files
%.dis: %.srec
	$(OBJDUMP) $(ODFLAGS) $*.srec > $*.dis

# how to disassemble original firmware file
%.dis: %.lgo
	$(OBJDUMP) $(ODFLAGS) $*.lgo > $*.dis

# how to merge labels etc into disassembly
%.s: %.fix $(FIRMVERS).dis
	./fixfirm.pl $*.fix $(FIRMVERS).dis > $*.s

# how to reassemble new firmware file
%.srec: %.s
	$(AS) $*.s -o $*.o
	$(LD) -Ttext=0x8000 $*.o -o $*.out
	$(OBJCOPY) -O srec $*.out $*.srec
	rm -f $*.o $*.out
	chmod -x $*.srec

# DO NOT DELETE - makedepend depends on this!

../source/conio.o: ../include/sim_conio.h ../include/config.h
../source/conio.o: ../include/rom/sim_lcd.h ../include/string.h
../source/conio.o: ../include/mem.h /usr/include/unistd.h
../source/conio.o: ../include/sim_mem.h ../include/sim_direct-lcd.h
../source/conio.o: ../include/sys/bitops.h
../source/direct-ir.o: ../include/sim_direct-ir.h ../include/config.h
../source/direct-ir.o: ../include/sim_mem.h ../include/sys/h8.h
../source/direct-ir.o: ../include/sys/irq.h ../include/sim_direct-lcd.h
../source/direct-ir.o: ../include/sys/bitops.h ../include/sim_conio.h
../source/direct-ir.o: ../include/rom/sim_lcd.h ../include/string.h
../source/direct-ir.o: ../include/mem.h /usr/include/unistd.h
../source/direct-ir.o: ../include/stdlib.h
../source/direct-motor.o: ../include/sim_direct-motor.h ../include/string.h
../source/direct-motor.o: ../include/mem.h ../include/sys/sim_h8.h
../source/direct-sensor.o: ../include/sim_direct-sensor.h
../source/direct-sensor.o: ../include/sys/sim_h8.h ../include/sys/bitops.h
../source/kmain.o: ../include/stdlib.h ../include/mem.h ../include/string.h
../source/kmain.o: ../include/sys/time.h ../include/time.h
../source/kmain.o: ../include/config.h ../include/sim_kmain.h
../source/kmain.o: ../include/sim_direct-button.h
../source/kmain.o: ../include/sim_direct-sensor.h ../include/sys/sim_h8.h
../source/kmain.o: ../include/sys/bitops.h ../include/sim_direct-ir.h
../source/kmain.o: ../include/sim_mem.h ../include/sim_tm.h
../source/lcd.o: ../include/rom/sim_lcd.h ../include/string.h
../source/lcd.o: ../include/mem.h /usr/include/unistd.h
../source/tm.o: ../include/stdlib.h ../include/mem.h ../include/sim_tm.h
../source/tm.o: ../include/sim_mem.h
