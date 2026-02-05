#include "tty.h"
#include "mem.h"

#ifndef SHELL_H
#define SHELL_H

void shell_init();
void shell_help();
void shell_ls();
void shell_cd();
void shell_loop();
void shell_reboot();
void shell_shutdown();

#endif
