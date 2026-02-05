#include "shell.h"

uint32_t current_shell_inode = 0;

void shell_ls()
{
	kprintf("\nLS");
}

void shell_help()
{
	kprintf("\nHELP");
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


