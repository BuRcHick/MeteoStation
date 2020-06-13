######################################
# source
######################################
CURR_DIR = $(shell pwd)
STM32GenDir = $(CURR_DIR)/backend/
STM32Makefile = $(STM32GenDir)Makefile
include $(STM32Makefile)

BUILD_DIR = $(CURR_DIR)/build

DRIVERS_DIR = $(CURR_DIR)/drivers/
SOURCES := $(shell find $(DRIVERS_DIR)src -name '*.c')
STM32_INCLUDES = $(C_INCLUDES:-I%=%)

C_SOURCES := $(addprefix $(STM32GenDir), $(C_SOURCES)) $(SOURCES)
ASM_SOURCES := $(addprefix $(STM32GenDir), $(ASM_SOURCES))
C_INCLUDES := $(addprefix -I$(STM32GenDir), $(STM32_INCLUDES)) -I$(DRIVERS_DIR)inc
LDSCRIPT := $(addprefix $(STM32GenDir), $(LDSCRIPT))

backend: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).elf
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000