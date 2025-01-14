BUILD_DIR=../build/kernel

C_FILES := $(wildcard *.c)
C_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_FILES))

all: make_dir $(C_OBJECTS)

make_dir:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	i386-elf-gcc -g -ffreestanding -c $< -o $@
