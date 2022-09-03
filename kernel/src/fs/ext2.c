#include <libk/string.h>
#include <libk/stdio.h>
#include <libk/math.h>
#include <heap.h>
#include <disc.h>
#include <ext2.h>

ext2_superblock_t* ext2_superblock;

void read_block(uint32_t block_num, void* block_ptr)
{
	uint32_t sectors_per_block = BLOCK_SIZE / SECTOR_SIZE;

	for (size_t i = 0; i < sectors_per_block; i++) {
		read_sector(block_num * sectors_per_block + i, (disc_sector_t*)block_ptr + i);
	}
}

void read_superblock(ext2_superblock_t* superblock)
{
	char block[1024];
	read_sector(2, (disc_sector_t*)block);
	read_sector(3, (disc_sector_t*)block + 1);
	memcpy(superblock, block, sizeof(ext2_superblock_t));
}

void ext2_init()
{
	ext2_superblock = (ext2_superblock_t*)kalloc(sizeof(ext2_superblock_t));
	read_superblock(ext2_superblock);
}

void read_bg_desc(uint32_t bg_desc, ext2_bg_desc_t* ext2_bg_desc)
{
	uint32_t starting_block_num = BLOCK_SIZE == 1024 ? 2 : 1;
	uint32_t bg_descs_per_block = BLOCK_SIZE / BG_DESC_SIZE;
	uint32_t block_num = starting_block_num + bg_desc / bg_descs_per_block;
	uint32_t block_index = bg_desc % bg_descs_per_block;

	char block[BLOCK_SIZE];
	read_block(block_num, block);
	memcpy(ext2_bg_desc, (char*)block + BG_DESC_SIZE * block_index, sizeof(ext2_bg_desc_t));
}

void read_inode(uint32_t starting_block_num, uint32_t inode_index, ext2_inode_t* ext2_inode)
{
	uint32_t block_num = starting_block_num + inode_index / INODES_PER_BLOCK;
	uint32_t block_index = inode_index % INODES_PER_BLOCK;

	char block[BLOCK_SIZE];
	read_block(block_num, block);
	memcpy(ext2_inode, (char*)block + block_index * INODE_SIZE, sizeof(ext2_inode_t));
}

list_t* directory_to_entries(uint32_t inode)
{
	list_t* list = NULL;

	uint32_t bg_desc = (inode - 1) / ext2_superblock->inodes_per_group;
	uint32_t inode_index = (inode - 1) % ext2_superblock->inodes_per_group;

	// block group descriptor
	ext2_bg_desc_t* ext2_bg_desc;
	ext2_bg_desc = (ext2_bg_desc_t*)kalloc(sizeof(ext2_bg_desc_t));
	read_bg_desc(bg_desc, ext2_bg_desc);

	// inode table
	ext2_inode_t* ext2_inode;
	ext2_inode = (ext2_inode_t*)kalloc(sizeof(ext2_inode_t));
	read_inode(ext2_bg_desc->inode_block_address, inode_index, ext2_inode);

	// if it is not directory
	if (!(ext2_inode->type_perms & TYPE_DIR))
		return list;

	// read inode contents
	for (size_t i = 0; i < 12; i++) {
		if (ext2_inode->dbp[i] == 0)
			break;

		// get block
		char block[BLOCK_SIZE];
		read_block(ext2_inode->dbp[i], block);

		// parse block
		for (size_t block_offset = 0; block_offset < BLOCK_SIZE;) {
			// get dentry header
			ext2_dentry_t* ext2_dentry;
			ext2_dentry = (ext2_dentry_t*)kalloc(sizeof(ext2_dentry_t));
			memcpy(ext2_dentry, (char*)block + block_offset, sizeof(ext2_dentry_t) - sizeof(char*));

			// get dentry name
			ext2_dentry->name = (char*)kalloc(ext2_dentry->name_length_lower);
			memcpy(ext2_dentry->name, (char*)block + block_offset + sizeof(ext2_dentry_t) - sizeof(char*), ext2_dentry->name_length_lower);

			if (inode == 0) {
				kfree(ext2_dentry->name);
				kfree(ext2_dentry);
				break;
			}

			// put dentry in list
			add_to_list_head(&list, ext2_dentry);

			// offset
			block_offset += ext2_dentry->size;
		}
	}


	kfree(ext2_bg_desc);
	kfree(ext2_inode);

	return list;
}

char* files_to_buffer(uint32_t inode)
{
	uint32_t bg_desc = (inode - 1) / ext2_superblock->inodes_per_group;
	uint32_t inode_index = (inode - 1) % ext2_superblock->inodes_per_group;

	// block group descriptor
	ext2_bg_desc_t* ext2_bg_desc;
	ext2_bg_desc = (ext2_bg_desc_t*)kalloc(sizeof(ext2_bg_desc_t));
	read_bg_desc(bg_desc, ext2_bg_desc);

	// inode table
	ext2_inode_t* ext2_inode;
	ext2_inode = (ext2_inode_t*)kalloc(sizeof(ext2_inode_t));
	read_inode(ext2_bg_desc->inode_block_address, inode_index, ext2_inode);

	char* data;

	uint32_t size = ext2_inode->size_lower;
	data = (char*)kalloc(size);

	uint32_t block_num = upper_div(size, BLOCK_SIZE);
	for (size_t i = 0; i < min(block_num, 12); i++) {
		char block[BLOCK_SIZE];
		read_block(ext2_inode->dbp[i], block);
		memcpy(data + i * BLOCK_SIZE, block, size >= (i + 1) * BLOCK_SIZE ? BLOCK_SIZE : size % BLOCK_SIZE);
	}

	return data;
}

list_t* path_to_list(const char* path)
{
	size_t i, j;
	list_t* divided_path = NULL;

	size_t n = strlen(path);
	for (i = 0, j = 0; i <= n; i++) {
		if (i == n || path[i] == '/') {
			// add data before slash
			if (i != j) {
				char* ptr = (char*)kalloc(sizeof(char) * (uint32_t)(i - j + 1));
				memcpy(ptr, path + j, i - j);
				ptr[i - j] = '\0';
				add_to_list_tail(&divided_path, ptr);
			}
			// add slash
			if (i != n) {
				char* ptr = (char*)kalloc(sizeof(char) * 2);
				ptr[0] = '/';
				ptr[1] = '\0';
				add_to_list_tail(&divided_path, ptr);
				j = i + 1;
			}
		}
	}

	return divided_path;
}

// only supports absolute path for now
uint32_t path_to_inode(const char* path)
{
	uint32_t inode = 0;
	list_t* divided_path = path_to_list(path);

	list_t* curr_dir = divided_path;

	// first entry is /
	curr_dir = curr_dir->next;
	inode = 2;

	while (curr_dir != NULL) {
		// list of dentry
		list_t* list = directory_to_entries(inode);

		// check if inode is actually a dir
		if (list == NULL) {
			printf("not a directory\n");
			return 0;
		}

		// iterate through all direntries
		uint8_t ind = 1;
		for (list_t* curr_list = list; curr_list != NULL; curr_list = curr_list->next) {
			ext2_dentry_t* ext2_dentry;
			ext2_dentry = curr_list->data;

			if (!memcmp(curr_dir->data, ext2_dentry->name)) {
				ind = 0;
				inode = ext2_dentry->inode;
				break;
			}
		}

		// if dir not found error
		if (ind) {
			printf("file/dir not found\n");
			return 0;
		}

		// next dir
		curr_dir = curr_dir->next;
		if (curr_dir != NULL)
			curr_dir = curr_dir->next;
	}

	return inode;
}

void ls(uint32_t inode)
{
	list_t* dir = directory_to_entries(inode);

	printf("ls dir with inode %d:\n", inode);
	for (list_t* tmp = dir; tmp != NULL; tmp = tmp->next) {
		ext2_dentry_t* ext2_dentry;
		ext2_dentry = tmp->data;
		printf("inode: %d, name: %s\n", ext2_dentry->inode, ext2_dentry->name);
	}
}

void print(uint32_t inode)
{
	printf("contents of inode %d:\n", inode);
	char *p = files_to_buffer(inode);
	printf("%s", p);
}
