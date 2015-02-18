# Make the build directory reflect the board name
BUILD = build/$(BOARD)/modx

MODX_INC =  -I.
MODX_INC += -I..
MODX_INC += -Ihal
MODX_INC += -Ihal/inc
MODX_INC += -I$(PY_SRC)
MODX_INC += -I$(BUILD)

MODX_CPPDEFINES = -Dgcc -DMODX -DTARGET_IS_CC3200

MODX_PORT_SRC_C = $(addprefix mods/modx/,\
	dummy.c \
	)

MODX_COMM_SRC_C = $(addprefix extmod/modx/,\
	modx.c \
	)

OBJ = $(addprefix $(BUILD)/, $(MODX_PORT_SRC_C:.c=.o) $(MODX_COMM_SRC_C:.c=.o))

# Define the linker script
LINKER_SCRIPT = ../extmod/modx/mpextern.ld
LDFLAGS += -T $(LINKER_SCRIPT)

# Add the modx specific CFLAGS
CFLAGS += $(MODX_CPPDEFINES) $(MODX_INC)
CFLAGS += -Os -DNDEBUG -fPIC

# Define the header build location
HEADER_BUILD = $(BUILD)/genhdr

# Create a list of all dynamic modules to be built
MODX = $(addprefix $(BUILD)/, modx.mpy dummy.mpy)

all: $(MODX)

# Link the common external modules
$(BUILD)/%.axf: $(BUILD)/extmod/modx/%.o
	$(ECHO) "LINK $@"
	$(Q)$(CC) -o $@ $(LDFLAGS) $<
	$(Q)$(SIZE) $@

# Link the port's external modules
$(BUILD)/%.axf: $(BUILD)/mods/modx/%.o
	$(ECHO) "LINK $@"
	$(Q)$(CC) -o $@ $(LDFLAGS) $<
	$(Q)$(SIZE) $@

$(BUILD)/%.mpy: $(BUILD)/%.axf
	$(ECHO) "Create $@"
	$(Q)$(OBJCOPY) -O binary -j .all $< $@

# Create an empty "qstrdefs.generated.h" needed by py/mkrules.mk
$(HEADER_BUILD)/qstrdefs.generated.h: | $(HEADER_BUILD)
	touch $@

# Create an empty "py-version.h" needed by py/mkrules.mk
$(HEADER_BUILD)/py-version.h: | $(HEADER_BUILD)
	touch $@

