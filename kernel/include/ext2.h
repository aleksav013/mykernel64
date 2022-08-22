#ifndef EXT2_H
#define EXT2_H

#include <types.h>

#include <libk/list.h>

struct ext2_superblock_t {
// base fields
	uint32_t inodes_count;
	uint32_t blocks_count;
	uint32_t superuser_blocks;
	uint32_t free_blocks;
	uint32_t free_inodes;
	uint32_t superblock_block;
	uint32_t block_size;
	uint32_t fragment_size;
	uint32_t blocks_per_group;
	uint32_t fragments_per_group;
	uint32_t inodes_per_group;
	uint32_t last_mount_time;
	uint32_t last_write_time;
	uint16_t mounts_since_fcsk;
	uint16_t mounts_before_fcsk;
	uint16_t signature;
	uint16_t state;
	uint16_t on_error;
	uint16_t version_minor;
	uint32_t last_time_fcsk;
	uint32_t interval_fcsk;
	uint32_t os_id;
	uint32_t version_major;
	uint16_t superuser_id;
	uint16_t supergroup_id;
// extended fields
	uint32_t first_inode;
	uint16_t inode_size;
	uint16_t superblock_group;
	uint32_t optional_features;
	uint32_t required_features;
	uint32_t ro_features;
	uint8_t id[16];
	uint8_t volume_name[16];
	uint8_t path_name[64];
	uint32_t compression;
	uint8_t blocks_for_files;
	uint8_t blocks_for_dirs;
	uint16_t unused;
	uint8_t journal_id[16];
	uint32_t journal_inode;
	uint32_t journal_device;
	uint32_t orphan_inode_head;
	// unused
};
typedef struct ext2_superblock_t ext2_superblock_t;

struct ext2_bg_desc_t {
	uint32_t block_bitmap;
	uint32_t inode_bitmap;
	uint32_t inode_block_address;
	uint16_t free_blocks;
	uint16_t free_inodes;
	uint16_t dir_count;
};
typedef struct ext2_bg_desc_t ext2_bg_desc_t;

struct ext2_inode_t {
	uint16_t type_perms;
	uint16_t user_id;
	uint32_t size_lower;
	uint32_t last_accessed;
	uint32_t creation_time;
	uint32_t last_modified;
	uint32_t deletion_time;
	uint16_t group_id;
	uint16_t hard_links_count;
	uint32_t sectors_used;
	uint32_t flags;
	uint8_t os_specific1[4];
	uint32_t dbp[12];
	uint32_t sibp;
	uint32_t dibp;
	uint32_t tibp;
	uint32_t gen_number;
	uint32_t eab;
	uint32_t size_upper;
	uint32_t fragment_address;
	uint8_t os_specific2[12];
};
typedef struct ext2_inode_t ext2_inode_t;

// inode type
#define TYPE_FIFO		0x1000
#define TYPE_CHAR_DEV		0x2000
#define TYPE_DIR		0x4000
#define TYPE_BLOCK DEV		0x6000
#define TYPE_FILE		0x8000
#define TYPE_SYMLINK		0xA000
#define TYPE_SOCKET		0xC000

// inode permission
#define PERM_OE 0x001
#define PERM_OR 0x002
#define PERM_OW 0x004
#define PERM_GE 0x008
#define PERM_GR 0x010
#define PERM_GW 0x020
#define PERM_UE 0x040
#define PERM_UR 0x080
#define PERM_UW 0x100
#define PERM_STICKY 0x200
#define PERM_GID 0x400
#define PERM_UID 0x800

struct ext2_dentry_t {
	uint32_t inode;
	uint16_t size;
	uint8_t name_length_lower;
	uint8_t type;
	char* name;
};
typedef struct ext2_dentry_t ext2_dentry_t;

extern ext2_superblock_t* ext2_superblock;

// size of structs
#define BG_DESC_SIZE 32
#define BLOCK_SIZE (uint32_t)(1024 << ext2_superblock->block_size)
#define INODE_SIZE (ext2_superblock->inode_size)
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)


void read_block(uint32_t block_num, void* block_ptr);
void read_superblock(ext2_superblock_t* ext2_superblock);
void read_bg_desc(uint32_t bg_desc, ext2_bg_desc_t* ext2_bg_desc);
void read_inode(uint32_t starting_block_num, uint32_t inode_index, ext2_inode_t* ext2_inode);
list_t* directory_to_entries(uint32_t inode);
char* files_to_buffer(uint32_t inode);
list_t* path_to_list(const char* path);
uint32_t path_to_inode(const char* path);
void ls(uint32_t inode);
void print(uint32_t inode);

#endif
