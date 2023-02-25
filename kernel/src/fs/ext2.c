#include <libk/string.h>
#include <libk/stdio.h>
#include <libk/math.h>
#include <heap.h>
#include <disc.h>
#include <ext2.h>

ext2_superblock_t *ext2_superblock;

void read_block(uint32_t block_num, void *block_ptr)
{
	uint32_t sectors_per_block = BLOCK_SIZE / SECTOR_SIZE;

	for (size_t i = 0; i < sectors_per_block; i++) {
		read_sector(block_num * sectors_per_block + i,
			    (disc_sector_t *)block_ptr + i);
	}
}

void read_superblock(ext2_superblock_t *superblock)
{
	char block[1024];
	read_sector(2, (disc_sector_t *)block);
	read_sector(3, (disc_sector_t *)block + 1);
	memcpy(superblock, block, sizeof(ext2_superblock_t));
}

void ext2_init()
{
	ext2_superblock =
		(ext2_superblock_t *)kalloc(sizeof(ext2_superblock_t));
	read_superblock(ext2_superblock);
}

void read_bg_desc(uint32_t bg_desc, ext2_bg_desc_t *ext2_bg_desc)
{
	uint32_t starting_block_num = BLOCK_SIZE == 1024 ? 2 : 1;
	uint32_t bg_descs_per_block = BLOCK_SIZE / BG_DESC_SIZE;
	uint32_t block_num = starting_block_num + bg_desc / bg_descs_per_block;
	uint32_t block_index = bg_desc % bg_descs_per_block;

	char block[BLOCK_SIZE];
	read_block(block_num, block);
	memcpy(ext2_bg_desc, (char *)block + BG_DESC_SIZE * block_index,
	       sizeof(ext2_bg_desc_t));
}

void read_inode(uint32_t starting_block_num, uint32_t inode_index,
		ext2_inode_t *ext2_inode)
{
	uint32_t block_num =
		starting_block_num + inode_index / INODES_PER_BLOCK;
	uint32_t block_index = inode_index % INODES_PER_BLOCK;

	char block[BLOCK_SIZE];
	read_block(block_num, block);
	memcpy(ext2_inode, (char *)block + block_index * INODE_SIZE,
	       sizeof(ext2_inode_t));
}

dentry_list_t *directory_to_entries(uint32_t inode)
{
	dentry_list_t *dentry_list =
		(dentry_list_t *)kalloc(sizeof(dentry_list_t));
	INIT_LIST(dentry_list->list);

	if (inode < 2) {
		return dentry_list;
	}

	uint32_t bg_desc = (inode - 1) / ext2_superblock->inodes_per_group;
	uint32_t inode_index = (inode - 1) % ext2_superblock->inodes_per_group;

	/* block group descriptor */
	ext2_bg_desc_t *ext2_bg_desc;
	ext2_bg_desc = (ext2_bg_desc_t *)kalloc(sizeof(ext2_bg_desc_t));
	read_bg_desc(bg_desc, ext2_bg_desc);

	/* inode table */
	ext2_inode_t *ext2_inode;
	ext2_inode = (ext2_inode_t *)kalloc(sizeof(ext2_inode_t));
	read_inode(ext2_bg_desc->inode_block_address, inode_index, ext2_inode);

	/* if it is not directory */
	if (!(ext2_inode->type_perms & TYPE_DIR))
		return dentry_list;

	/* read inode contents */
	for (size_t i = 0; i < 12; i++) {
		if (ext2_inode->dbp[i] == 0)
			break;

		/* get block */
		char block[BLOCK_SIZE];
		read_block(ext2_inode->dbp[i], block);

		/* parse block */
		for (size_t block_offset = 0; block_offset < BLOCK_SIZE;) {
			/* get dentry header */
			dentry_list_t *ext2_dentry_list =
				(dentry_list_t *)kalloc(sizeof(dentry_list_t));
			memcpy(&ext2_dentry_list->ext2_dentry,
			       (char *)block + block_offset,
			       sizeof(ext2_dentry_t) - sizeof(char *));

			/* dentry is unused */
			if (ext2_dentry_list->ext2_dentry.inode == 0) {
				kfree(ext2_dentry_list);
				continue;
			}

			/* get dentry name */
			ext2_dentry_list->ext2_dentry.name = (char *)kalloc(
				ext2_dentry_list->ext2_dentry.name_length_lower +
				1);
			memcpy(ext2_dentry_list->ext2_dentry.name,
			       (char *)block + block_offset +
				       sizeof(ext2_dentry_t) - sizeof(char *),
			       ext2_dentry_list->ext2_dentry.name_length_lower);
			ext2_dentry_list->ext2_dentry
				.name[ext2_dentry_list->ext2_dentry
					      .name_length_lower] = '\0';

			/* put dentry in list */
			add_to_list(&ext2_dentry_list->list, &dentry_list->list,
				    dentry_list->list.next);

			/* offset */
			block_offset += ext2_dentry_list->ext2_dentry.size;
		}
	}

	kfree(ext2_bg_desc);
	kfree(ext2_inode);

	return dentry_list;
}

char *files_to_buffer(uint32_t inode)
{
	if (inode < 2) {
		return NULL;
	}

	uint32_t bg_desc = (inode - 1) / ext2_superblock->inodes_per_group;
	uint32_t inode_index = (inode - 1) % ext2_superblock->inodes_per_group;

	/* block group descriptor */
	ext2_bg_desc_t *ext2_bg_desc;
	ext2_bg_desc = (ext2_bg_desc_t *)kalloc(sizeof(ext2_bg_desc_t));
	read_bg_desc(bg_desc, ext2_bg_desc);

	/* inode table */
	ext2_inode_t *ext2_inode;
	ext2_inode = (ext2_inode_t *)kalloc(sizeof(ext2_inode_t));
	read_inode(ext2_bg_desc->inode_block_address, inode_index, ext2_inode);

	if (ext2_inode->type_perms & TYPE_DIR) {
		printf("can't print directory\n");
		kfree(ext2_bg_desc);
		kfree(ext2_inode);
		return NULL;
	}

	char *data;

	uint32_t size = ext2_inode->size_lower;
	data = (char *)kalloc(size + 1);

	uint32_t block_num = upper_div(size, BLOCK_SIZE);
	for (size_t i = 0; i < min(block_num, 12); i++) {
		char block[BLOCK_SIZE];
		read_block(ext2_inode->dbp[i], block);
		memcpy(data + i * BLOCK_SIZE, block,
		       size >= (i + 1) * BLOCK_SIZE ? BLOCK_SIZE :
						      size % BLOCK_SIZE);
	}
	data[size] = '\0';

	kfree(ext2_bg_desc);
	kfree(ext2_inode);

	return data;
}

path_t *path_to_list(const char *path)
{
	size_t i, j;
	path_t *divided_path = (path_t *)kalloc(sizeof(path_t));
	INIT_LIST(divided_path->list)

	size_t n = strlen(path);
	for (i = 0, j = 0; i <= n; i++) {
		if (i == n || path[i] == '/') {
			/* add data before slash */
			if (i != j) {
				path_t *curr_path =
					(path_t *)kalloc(sizeof(path_t));
				curr_path->name = (char *)kalloc(
					sizeof(char) * (uint32_t)(i - j + 1));
				memcpy(curr_path->name, path + j, i - j);
				curr_path->name[i - j] = '\0';
				add_to_list(&curr_path->list,
					    &divided_path->list,
					    divided_path->list.next);
			}
			/* add slash */
			if (i != n) {
				path_t *curr_path =
					(path_t *)kalloc(sizeof(path_t));
				curr_path->name =
					(char *)kalloc(sizeof(char) * 2);
				curr_path->name[0] = '/';
				curr_path->name[1] = '\0';
				add_to_list(&curr_path->list,
					    &divided_path->list,
					    divided_path->list.next);
				j = i + 1;
			}
		}
	}

	return divided_path;
}

uint32_t path_to_inode(const char *path)
{
	uint32_t inode = 0;
	path_t *divided_path = path_to_list(path);

	/* first entry is / */
	path_t *curr_path = list_prev_entry(divided_path, list);
	curr_path = list_prev_entry(curr_path, list);
	inode = 2;

	while (curr_path != divided_path) {
		/* list of dentry */
		dentry_list_t *dentry_list = directory_to_entries(inode);

		/* check if inode is actually a dir */
		if (list_is_empty((&dentry_list->list))) {
			printf("not a directory\n");
			return 0;
		}

		/* iterate through all direntries */
		uint8_t ind = 1;
		dentry_list_t *curr_dir;
		list_for_each_entry_prev(curr_dir, (&dentry_list->list), list) {
			if (!memcmp(curr_dir->ext2_dentry.name,
				    curr_path->name)) {
				ind = 0;
				inode = curr_dir->ext2_dentry.inode;
				break;
			}
		}

		/* if dir not found error */
		if (ind) {
			printf("file/dir not found\n");
			return 0;
		}

		/* next dir */
		curr_path = list_prev_entry(curr_path, list);
		if (curr_path != divided_path)
			curr_path = list_prev_entry(curr_path, list);
	}

	path_t *pos;
	list_for_each_entry_del(pos, (&divided_path->list), list) {
		kfree(pos->name);
		kfree(pos);
	}
	kfree(divided_path);

	return inode;
}

void ls(uint32_t inode)
{
	dentry_list_t *dir = directory_to_entries(inode);
	if (list_is_empty((&dir->list))) {
		return;
	}

	printf("ls dir with inode %d:\n", inode);
	dentry_list_t *pos;
	list_for_each_entry(pos, (&dir->list), list) {
		printf("inode: %d, name: %s\n", pos->ext2_dentry.inode,
		       pos->ext2_dentry.name);
	}

	list_for_each_entry_del(pos, (&dir->list), list) {
		kfree(pos->ext2_dentry.name);
		kfree(pos);
	}
	kfree(dir);
}

void print_inode(uint32_t inode)
{
	char *p = files_to_buffer(inode);
	if (p != NULL) {
		printf("contents of inode %d:\n", inode);
		printf("%s", p);
		kfree(p);
	}
}
