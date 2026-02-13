#include "fs.h"
#include "../kernel/mem.h"
#include "../kernel/tty.h"

uint32_t ext_block_size = 0;
uint32_t ext_block_per_group = 0;
uint32_t ext_inodes_per_group = 0;
uint32_t ext_group_count = 0;

void initialize_fs()
{
	uint8_t* super_block = (uint8_t*)(kmalloc(1024));
	read_ata(EXT_PARTITION_START + 2, 2, super_block); /* SUPERBLOCK IS ALWAYS 1024 */

	struct fs_superblock* block = (struct fs_superblock*)(super_block);
	
	if (block->signature == EXT_SIGNATURE) {
		kprintf("EXT SIGNATURE FOUNDED\n");
	}
		
	ext_block_size = (block->block_size + 1) * 1024;
	ext_block_per_group = (block->block_per_group);
	ext_inodes_per_group = (block->inodes_per_group);
	ext_group_count = (block->block_count / ext_block_per_group);

	kprintf("INODE PER GROUP: %d\n", ext_inodes_per_group);
}

uint32_t find_i_group(uint32_t inode_num)
{
	return ((inode_num - 1) / ext_inodes_per_group);
}

uint32_t find_i_index(uint32_t inode_num)
{
	return ((inode_num - 1) % ext_inodes_per_group);
}

void ext_descr_table_read(uint32_t group_num, uint32_t* bmap, uint32_t* imap, uint32_t* itable)
{
	uint32_t block_space = (ext_group_count * sizeof(struct block_descr_table));
	uint32_t sectors = (block_space + 511) / 512;
	uint8_t* block_table = (uint8_t*)(kmalloc(sectors * 512));
	read_ata(EXT_PARTITION_START + 4, sectors, block_table);
	struct block_descr_table* block_array = (struct block_descr_table*)(block_table);
	
	//kprintf("%d, %d\n", EXT_PARTITION_START + 4, block_space);
	
	if(group_num > ext_group_count) {
		kprintf("Incorrect block number\n");
		return;
	}

	*bmap = block_array[group_num].bmap_addr;
	*imap = block_array[group_num].imap_addr;
	*itable = block_array[group_num].itable_addr;

	kprintf("%d, %d, %d\n", *bmap, *imap, *itable);
	//kfree(block_table);
}

void inode_read(uint32_t inode_num, struct inode_t* inode)
{
	uint32_t i_gr = find_i_group(inode_num);
	uint32_t i_ind = find_i_index(inode_num);
	
	kprintf("%d, %d, %d\n", inode_num, i_gr, i_ind);

	uint32_t bmap_addr, imap_addr, itable_addr;
	ext_descr_table_read(i_gr, &bmap_addr, &imap_addr, &itable_addr);
		
	uint8_t inode_table_temp = (uint8_t*)(kmalloc(512));
	read_ata(EXT_PARTITION_START + (itable_addr * (ext_block_size / 512)) + (i_ind / 4), 1, inode_table_temp);
	struct inode_t* inode_table = (struct inode_t*)(inode_table_temp);
	*inode = inode_table[i_ind % 4];
	//kfree(inode_table_temp);
}

void dir_lookup(struct inode_t* dir, char* child_name, struct inode_t* inode_ret)
{
	uint8_t* child_names = (uint8_t*)(kmalloc(1024));
	kprintf("DIR BLOCK, DIR BLOCK %d\n", dir->block_ptr[0]);
	for(int i = 0; i < 12; i++) {
		if(dir->block_ptr[i] == 0)
			break;
		
		kprintf("HEJ WOJTEK");
		read_ata(EXT_PARTITION_START + (dir->block_ptr[i] * (ext_block_size / 512)), 2, child_names);
		uint32_t offset = 0;
		int find = 0;
		while (offset < 1024) {
			struct dir_entry_t* entry = (struct dir_entry_t*)(child_names + offset);

			char name[256];
			memcpy(name, entry->name, entry->name_len);
			name[entry->name_len] = 0;

			kprintf("%s %d\n", name, entry->inode);
			uint32_t num = entry->inode;

			if(strcmp(child_name, name) == 0) {
				inode_read(num, inode_ret);
				kprintf("ENTRY INODE: %d, NAME: %s\n %d", num, name, inode_ret->block_ptr[0]);
				find = 1;
				break;
			}

			if(entry->size == 0) {
				break;
			}

			offset += entry->size;
		}

		if(find == 1) {
			break;
		}
	}

	//kfree(child_names);
}

void namei_at(struct inode_t* start, char* path, struct inode_t* inode_ret)
{
	kprintf("PATH: %s\n", path);

	char name[256];
	char curr_character = path[0];
	uint32_t curr_len = 0;
	uint32_t curr_path = 0;
	struct inode_t* current_inode = start;

	while(curr_character != '\0') {
		while(curr_character != '/' && curr_character != '\0') {
			kprintf("%c ", curr_character);
			name[curr_len] = curr_character;
			curr_len++;
			curr_path++;
			curr_character = path[curr_path];
		}
		
		if(curr_character == '\0')
			break;
		
		name[curr_len] = '\0';
		kprintf("NAMENAME1: %s\n", name);	
		dir_lookup(current_inode, name, inode_ret);
		current_inode = inode_ret;
		curr_len = 0;
		curr_path++;
		curr_character = path[curr_path];
		kprintf("SCHIZOID");
	}
	
	name[curr_len] = '\0';	
	kprintf("NAMENAME: %s\n", name);
	kprintf("%d, %d\n", start, current_inode);
	dir_lookup(current_inode, name, inode_ret);
	kprintf("DIR BLOCK %d\n", inode_ret->block_ptr[0]);
}

void namei(char* path, struct inode_t* inode_ret)
{
	struct inode_t inode;
	inode_read(2, &inode);
	namei_at(&inode, path+1, inode_ret);
}



