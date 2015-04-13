include ../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h #$(BUILD)/pins_qstr.h

# include py core make definitions
include ../py/py.mk

INC =  -I.
INC += -I..
INC += -I../stmhal
INC += -I../lib/mp-readline
INC += -I$(BUILD)
INC += -Iinclude


CFLAGS = $(INC) -Wall -Wpointer-arith -ansi -std=gnu99 -nostdlib \
	-march=armv5te -mfloat-abi=soft -Wall -Iinclude -marm \
	-fshort-enums \
	 $(COPT)

LDFLAGS = -nostdlib -T fernvale.ld
LIBS = -Llib

LIBGCC_FILE_NAME = $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)
LIBS += lib/libm.a lib/libgcc-armv5.a

# Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -g
COPT = -O0
else
CFLAGS += 
COPT += -Os -DNDEBUG
LDFLAGS += --gc-sections
endif

SRC_C = \
	strtoll.c \
	main.c \
	fernvale_mphal.c \
	gccollect.c \
	serial.c \
	modpyb.c \

STM_SRC_C = $(addprefix stmhal/,\
	printf.c \
	pyexec.c \
	help.c \
	pybstdio.c \
	)

LIB_SRC_C = $(addprefix lib/,\
	libc/string0.c \
	mp-readline/readline.c \
	)

SRC_S = \
	gchelper.s \
	startup_mt6260.s \

OBJ =
OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/, $(STM_SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))
#OBJ += $(BUILD)/pins_$(BOARD).o

all: $(BUILD)/firmware.bin

.PHONY: deploy

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -S -O binary $(BUILD)/firmware.elf $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@

#MAKE_PINS = boards/make-pins.py
#BOARD_PINS = boards/$(BOARD)/pins.csv
#AF_FILE = boards/stm32f4xx_af.csv
#PREFIX_FILE = boards/stm32f4xx_prefix.c
#GEN_PINS_SRC = $(BUILD)/pins_$(BOARD).c
#GEN_PINS_HDR = $(HEADER_BUILD)/pins.h
#GEN_PINS_QSTR = $(BUILD)/pins_qstr.h
#GEN_PINS_AF_CONST = $(HEADER_BUILD)/pins_af_const.h
#GEN_PINS_AF_PY = $(BUILD)/pins_af.py

# Making OBJ use an order-only depenedency on the generated pins.h file
# has the side effect of making the pins.h file before we actually compile
# any of the objects. The normal dependency generation will deal with the
# case when pins.h is modified. But when it doesn't exist, we don't know
# which source files might need it.
#$(OBJ): | $(HEADER_BUILD)/pins.h

# Use a pattern rule here so that make will only call make-pins.py once to make
# both pins_$(BOARD).c and pins.h
#$(BUILD)/%_$(BOARD).c $(HEADER_BUILD)/%.h $(HEADER_BUILD)/%_af_const.h $(BUILD)/%_qstr.h: boards/$(BOARD)/%.csv $(MAKE_PINS) $(AF_FILE) $(PREFIX_FILE) | $(HEADER_BUILD)
#	$(ECHO) "Create $@"
#	$(Q)$(PYTHON) $(MAKE_PINS) --board $(BOARD_PINS) --af $(AF_FILE) --prefix $(PREFIX_FILE) --hdr $(GEN_PINS_HDR) --qstr $(GEN_PINS_QSTR) --af-const $(GEN_PINS_AF_CONST) --af-py $(GEN_PINS_AF_PY) > $(GEN_PINS_SRC)
#
#$(BUILD)/pins_$(BOARD).o: $(BUILD)/pins_$(BOARD).c
#	$(call compile_c)

include ../py/mkrules.mk
