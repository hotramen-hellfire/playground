#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

void panic(char *msg)
{
	printf("%s\n", msg);
	fflush(stdout);
	assert(0);
	return;//never return
}

int simplefs_create(char *filename)
{
	struct inode_t *inodeptr;
	for (int i = 0; i < NUM_INODES; i++)
	{
			inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));
			simplefs_readInode(i, inodeptr);
			if (inodeptr->status==INODE_IN_USE && strcmp(inodeptr->name, filename) == 0)
			{
				free(inodeptr);
				return -1;
			}
			free(inodeptr);
	}
	int index = simplefs_allocInode();
	if (index < 0)
		return -1;
	inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));
	strncpy(inodeptr->name, filename, MAX_NAME_STRLEN-1);
	inodeptr->file_size = 0;
	inodeptr->status = INODE_IN_USE;
	for (int i = 0; i < MAX_FILE_SIZE; i++)
	{
		inodeptr->direct_blocks[i]=-1;
	}
	simplefs_writeInode(index, inodeptr);
	free(inodeptr);
	return index;
}

void simplefs_delete(char *filename)
{
	int idx=-1;
	struct inode_t *inodeptr;
	for (int i = 0; i < NUM_INODES; i++)
	{
			
			inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));
			simplefs_readInode(i, inodeptr);
			if (inodeptr->status==INODE_IN_USE && strcmp(inodeptr->name, filename) == 0)
			{
				idx=i;//set the index
				break;
			}
			free(inodeptr);
	}
	if (idx==-1) return;//filename not found
	// else we have the filename inodeptr
	for (int i=0; i<MAX_FILE_SIZE; i++)
	{
		if (inodeptr->direct_blocks[i]!=-1) simplefs_freeDataBlock(inodeptr->direct_blocks[i]);//free all datablocks
	}
	simplefs_freeInode(idx);
	return;
}

int simplefs_open(char *filename)
{
	// no testcase for this alone
	int idx=-1;
	struct inode_t *inodeptr;
	for (int i = 0; i < NUM_INODES; i++)
	{
			inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));
			simplefs_readInode(i, inodeptr);
			if (inodeptr->status==INODE_IN_USE && strcmp(inodeptr->name, filename) == 0)
			{
				idx=i;//set the index
				break;
			}
			free(inodeptr);
	}
	if (idx<0) return -1;
	//idx is the index of the file to be opened
	int fil_idx=-1;
	for (int pos=0; pos<MAX_OPEN_FILES; pos++)
	{
		if (file_handle_array[pos].inode_number==-1)
		{
			fil_idx=pos;
			break;
		}
	}
	if (fil_idx<0) return -1;
	file_handle_array[fil_idx].inode_number=idx;
	file_handle_array[fil_idx].offset=0;
	return fil_idx;
}

void simplefs_close(int file_handle)
{
	if (file_handle<0 || file_handle>=MAX_OPEN_FILES) return;
	if (file_handle_array[file_handle].inode_number<0) return;
	file_handle_array[file_handle].inode_number=-1;
	file_handle_array[file_handle].offset=0;
	return;
}


int simplefs_read(int file_handle, char *buf, int nbytes)
{
	int retidx=0;
	if (file_handle<0 || file_handle>=MAX_OPEN_FILES) return -1;
	if (file_handle_array[file_handle].inode_number<0) return -1;
	char* middle=(char*)malloc(nbytes);
	memset((void*)middle, 0, nbytes);//zero the memory
	struct inode_t *inodeptr;
	inodeptr = (struct inode_t *)malloc(sizeof(struct inode_t));
	simplefs_readInode(file_handle_array[file_handle].inode_number, inodeptr);
	int offset=file_handle_array[file_handle].offset;
	for (int p=offset; p<offset+nbytes; p+=BLOCKSIZE)
	{
		int thisp=p;
		char* blockbuff= (char*)malloc(BLOCKSIZE);
		int thisblock=p/BLOCKSIZE;
		if (inodeptr->direct_blocks[thisblock]>-1) simplefs_readDataBlock(inodeptr->direct_blocks[thisblock], blockbuff);
		else {free(middle); free(inodeptr); panic("jaay bhokat!!"); return -1;}//buff not updated
		while (retidx<nbytes && thisp!=(thisblock+1)*BLOCKSIZE){
			middle[retidx]=blockbuff[thisp%BLOCKSIZE];
			thisp++;
			retidx++;
		}
		if (retidx==nbytes)
		{
			if (thisp!=offset+nbytes) panic("ye kaise hogya bhai!!");;//this must hold otherwise "jaay bhokat!"
			break;
		}
	}
	memcpy(buf, middle, nbytes);
	return 0;
}

int simplefs_write(int file_handle, char *buf, int nbytes)
{
	/*
		write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
	return -1;
}

int simplefs_seek(int file_handle, int nseek)
{
	/*
	   increase `file_handle` offset by `nseek`
	*/
	return -1;
}