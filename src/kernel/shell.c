#include "shell.h"
#include "../fs/fs.h"
#include "../libc/include/string.h"

uint32_t current_shell_inode = 2;
char current_shell_dir[256];

void shell_ls()
{
	struct inode_t root_inode;
	namei("/kucharz", &root_inode);
	//inode_read(current_shell_inode, &root_inode);

	uint8_t* child_names = (uint8_t*)(kmalloc(1024));
	read_ata(2048 + (root_inode.block_ptr[0] * 2), 2, child_names);

	uint32_t offset = 0;
	
	while (offset < 1024) {
		struct dir_entry_t* entry = (struct dir_entry_t*)(child_names + offset);

		char name[256];
		memcpy(name, entry->name, entry->name_len);
		name[entry->name_len] = 0;

		kprintf("%s ", name);
		offset += entry->size;

		if(entry->size == 0) {
			break;
		}
	}	
}

void shell_help()
{
	struct inode_t inode;
	namei("/boot/grub/grub.cfg", &inode);
	uint8_t* names = (uint8_t*)(kmalloc(1024));
	read_ata(2048 + (inode.block_ptr[0] * 2), 2, names);
	kprintf(names);
}

void shell_loop()
{
	uint8_t buf[100];
	uint32_t len = 0;
	kprintf("\n> ");

	while(1) {
		uint8_t character = read_keyboard();
		
		if(character != '\n') {
			if(character != 0) {
				buf[len] = character;
				len++;
				kprintf("%c", buf[len - 1]);
			}
		}

		else 
		{
			buf[len] = '\0';
			
			if(strcmp(buf, "help") == 0) {
				shell_help();
			}

			else if(strcmp(buf, "ls") == 0) {
				shell_ls();
			}

			else
			{
				kprintf("\nBAD COMMAND, USE HELP TO SEE COMMANDS");
			}

			len = 0;
			kprintf("\n> ");

			for(uint32_t i = 0; i < 100; i++) {
				buf[i] = 0;
			}
		}
	}
}


