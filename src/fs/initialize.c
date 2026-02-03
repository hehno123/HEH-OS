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
	uint32_t block_space = (ext_group_count * sizeof(struct block_descr_table)) / 512;
	block_space++;
	uint8_t* block_table = (uint8_t*)(kmalloc(block_space));
	read_ata(EXT_PARTITION_START + 4, block_space, block_table);
	struct block_descr_table* block_array = (struct block_descr_table*)(block_table);
	
	kprintf("%d, %d\n", EXT_PARTITION_START + 4, block_space);

	if(group_num > ext_group_count) {
		kprintf("Incorrect block number\n");
		return;
	}

	*bmap = block_array[group_num].bmap_addr;
	*imap = block_array[group_num].imap_addr;
	*itable = block_array[group_num].itable_addr;

	kprintf("%d, %d, %d\n", *bmap, *imap, *itable);
}

void inode_read(uint32_t inode_num, struct inode_t* inode)
{
	uint32_t i_gr = find_i_group(inode_num);
	uint32_t i_ind = find_i_index(inode_num);
	
	kprintf("%d, %d\n", i_gr, i_ind);

	uint32_t bmap_addr, imap_addr, itable_addr;
	ext_descr_table_read(i_gr, &bmap_addr, &imap_addr, &itable_addr);
		
	uint8_t inode_table_temp = (uint8_t*)(kmalloc(512));
	read_ata(EXT_PARTITION_START + (itable_addr * (ext_block_size / 512)) + (i_ind / 4), 1, inode_table_temp);
	struct inode_t* inode_table = (struct inode_t*)(inode_table_temp);
	*inode = inode_table[i_ind % 4];
}



