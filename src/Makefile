BUILD_DIR = build
all: run_system

C_OBJECTS = $(shell find $(BUILD_DIR) -name '*.o')

$(BUILD_DIR)/entry.o: entry.asm
	nasm -g -f elf $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o ${C_OBJECTS}
	i386-elf-ld -o $@ -Ttext 0x10000 $^ --oformat binary

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/entry.o ${C_OBJECTS}
	i386-elf-ld -g -o $@ -Ttext 0x10000 $^

$(BUILD_DIR)/os-img.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $^ > $@

run_system: $(BUILD_DIR)/os-img.bin
	qemu-system-i386 -hda $< -s -S -monitor stdio

$(BUILD_DIR)/%.bin: %.asm
	nasm $< -g -f bin -o $@

clean:
	rm -rf $(BUILD_DIR)/*


