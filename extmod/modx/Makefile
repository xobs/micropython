INC =  -I. -I../..

CFLAGS = $(INC) -Wall -Werror -ansi -std=gnu99 -fPIC -Os

all: modx.mpy

modx.mpy: modx.elf
	objcopy -O binary -j .all $< $@

modx.elf: modx.o
	$(LD) -T mpextern.ld -o $@ $<
