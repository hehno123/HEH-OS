BUILD_DIR = build
all: run_system

C_OBJECTS = $(shell find $(BUILD_DIR) -name '*.o')

$(BUILD_DIR)/entry.o: entry.asm
	nasm -g -f elf $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o ${C_OBJECTS}
	i386-elf-gcc -o $@ -T linker.ld -ffreestanding -nostdlib $^ -lgcc

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/entry.o ${C_OBJECTS}
	i386-elf-gcc -ffreestanding -nostdlib -g -o $@ -Ttext 0x10000 $^ -lgcc

$(BUILD_DIR)/os-img.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $^ > $@

run_system: $(BUILD_DIR)/os-img.bin
	qemu-system-i386 -hda $< -s -S -m 4096 -monitor stdio

$(BUILD_DIR)/%.bin: %.asm
	nasm $< -g -f bin -o $@

clean:
	rm -rf $(BUILD_DIR)/*


