#ifndef _FS_H
#define _FS_H

#include <stdint.h>

#define EXT_SYSTEM_CLEAN 1
#define EXT_SYSTEM_ERR 2

#define EXT_IGNORE_ERR 1
#define EXT_REMOUNT_ERR 2
#define EXT_KERNEL_PANIC 3

#define EXT_SIGNATURE 0xEF53
#define EXT_PARTITION_START 2048


struct __attribute__((packed)) fs_superblock {
	uint32_t inode_count;
	uint32_t block_count;
	uint32_t s_block_res;
	uint32_t free_block_count;
	uint32_t free_inode_count;
	uint32_t superblock_num;
	uint32_t block_size;
	uint32_t fragment_size;
	uint32_t block_per_group;
	uint32_t fragments_per_group;
	uint32_t inodes_per_group;
	uint32_t last_mount;
	uint32_t last_written;
	uint16_t mounted_fsck;
	uint16_t allowed_mounts;
	uint16_t signature;
	uint16_t system_state;
	uint16_t err_detect;
	uint16_t minor_version;
	uint32_t last_fsck;
	uint32_t gap_fsck;
	uint32_t os_id;
	uint32_t rev_level;
	uint16_t user_id;
	uint16_t group_id;
};

struct __attribute__((packed)) block_descr_table {
	uint32_t bmap_addr;
	uint32_t imap_addr;
	uint32_t itable_addr;
	uint32_t free_blocks_count;
	uint32_t free_inodes_count;
	uint16_t dir_count;
	uint16_t pad;
	uint8_t reserved[12];
};

struct __attribute__((packed)) inode_t {
	uint16_t type_perms;
	uint16_t uid;
	uint32_t low_size;
	uint32_t last_access;
	uint32_t create_time;
	uint32_t last_mod;
	uint32_t deletion_time;
	uint16_t guid;
	uint16_t hard_links;
	uint32_t sec_count;
	uint32_t flags;
	uint32_t os_val;
	uint32_t block_ptr[15];
	uint32_t gen_num;
	uint32_t res1;
	uint32_t res2;
	uint32_t frag_addr;
	uint8_t os_val2[12];
};

struct __attribute__((packed)) dir_entry_t {
	uint32_t inode;
	uint16_t size;
	uint8_t name_len;
	uint8_t type;
	uint8_t name[];
};

void inode_read(uint32_t, struct inode_t*);
void initialize_fs();
void namei(char*, struct inode_t*);

#endif
