BUILD_DIR = build
all: run_system

C_OBJECTS = $(shell find $(BUILD_DIR) -name '*.o')

$(BUILD_DIR)/entry.o: kernel/boot.s
	i386-elf-as $< -o $@

$(BUILD_DIR)/kernel.o: $(BUILD_DIR)/entry.o ${C_OBJECTS}
	i386-elf-gcc -T linker.ld -o $@ -g -ffreestanding -O2 -nostdlib $^ -lgcc

$(BUILD_DIR)/disk.img: $(BUILD_DIR)/kernel.o
	dd if=/dev/zero of=$@ bs=512 count=131072
	parted -s $@ mklabel msdos
	parted -s $@ mkpart primary 1MiB 100%
	parted -s $@ set 1 boot on
	sudo losetup /dev/loop0 $@
	sudo losetup /dev/loop1 $@ -o 1048576
	sudo mke2fs -E revision=0 /dev/loop1
	sudo mount /dev/loop1 /mnt
	sudo mkdir -p /mnt/boot/grub
	sudo mkdir -p /mnt/kucharz
	sudo cp siema.txt /mnt/kucharz/siema.txt
	sudo cp $^ /mnt/boot 
	echo 'set debug=all' > grub.cfg
	echo 'menuentry "myos" {' > grub.cfg
	echo '  multiboot /boot/kernel.o' >> grub.cfg
	echo '}' >> grub.cfg
	sudo cp grub.cfg /mnt/boot/grub/grub.cfg
	sudo cp siema.txt /mnt/siema.txt
	rm grub.cfg
	sudo grub-install --target=i386-pc --root-directory=/mnt --no-floppy --modules="normal part_msdos ext2 multiboot" /dev/loop0
	sync
	sudo umount /mnt
	sudo losetup -d /dev/loop0
	sudo losetup -d /dev/loop1

run_system: $(BUILD_DIR)/disk.img
	qemu-system-i386 -m 4G -vga std -hda $< -s -S -monitor stdio -d int,in_asm

$(BUILD_DIR)/%.bin: %.asm
	nasm $< -g -f bin -o $@

clean:
	rm -rf $(BUILD_DIR)/*
	make -C drivers
	make -C kernel
	make -C cpu
	make -C libc/string
	make -C fs


