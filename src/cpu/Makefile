BUILD_DIR=../build/cpu

C_FILES := $(wildcard *.c)
ASM_FILES := $(wildcard *.asm)

C_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_FILES))
ASM_OBJECTS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(ASM_FILES))

OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

all: make_dir $(OBJECTS)

make_dir:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.asm
	as -32 $< -o $@

$(BUILD_DIR)/%.o: %.c 
	i386-elf-gcc -g -ffreestanding -c $< -o $@
