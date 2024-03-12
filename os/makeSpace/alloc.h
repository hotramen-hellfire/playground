#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define PAGESIZE 4096 //size of memory to allocate from OS
#define MINALLOC 8 //allocations will be 8 bytes or multiples of it

// function declarations
int init_alloc();
int cleanup();
char *alloc(int);
void dealloc(char *);
void _chunkstat(void);

struct chunk{
    int size;
    struct chunk* next_chunk;
    int start_offset;
    bool inuse;
};

struct paper{
    struct chunk* start_chunk;
    void* base_ptr;
    int size_unallocated;
};

bool hasPage = false;
struct paper* paper_point=nullptr;


void dealloc(char * ptr)
{
    if (hasPage==false)
    {
        printf("no page exists!!\n");
        return;
    }
    if ((int)(ptr-(char*)paper_point->base_ptr)>PAGESIZE-1)
    {
        return;
    }
    struct chunk* current_chunk = paper_point->start_chunk;
    struct chunk* prev_chunk = 0x0;
    bool found=false;
    while(current_chunk!=0X0 && found==false)
    {
        if ((current_chunk->start_offset+(char*)paper_point->base_ptr)==ptr)
        {
            found=true;
            current_chunk->inuse=false;
            if (current_chunk->next_chunk!=0x0 && current_chunk->next_chunk->inuse==false)
            {
                //coalesce
                current_chunk->size=current_chunk->next_chunk->size+current_chunk->size;
                struct chunk* to_be_deleted = current_chunk->next_chunk;
                current_chunk->next_chunk=current_chunk->next_chunk->next_chunk;
                //start offset remains the same
                //free the next chunk
                free(to_be_deleted);
            }
            if (prev_chunk!=0x0 && prev_chunk->inuse==false)
            {
                //coalesce
                prev_chunk->size=current_chunk->size+prev_chunk->size;
                prev_chunk->next_chunk=current_chunk->next_chunk;
                free(current_chunk);
            }
        }
        else
        {
            // Napoleon: there's nothing we can do
            prev_chunk=current_chunk;
            current_chunk=current_chunk->next_chunk;
        }
    }
    if (found==false) printf("no chunk to dealloc()!!\n");
    return;
}

int init_alloc()
{
    if (hasPage==false)
    {
    void *ptr = mmap(0, PAGESIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if (ptr==MAP_FAILED)
    {
        perror("mmap failed: ");
        fflush(stdout);
        return errno;
    }
    hasPage=true;
    paper_point=(paper*)malloc(sizeof(struct paper));
    paper_point->start_chunk=(chunk*)malloc(sizeof(struct chunk));
    paper_point->start_chunk->size=PAGESIZE;
    paper_point->start_chunk->inuse=false;
    paper_point->start_chunk->next_chunk=0x0;
    paper_point->start_chunk->start_offset=0;
    paper_point->size_unallocated=PAGESIZE;
    paper_point->base_ptr=ptr;
    return 0;
    }
    else
    {
        printf("a page has already been assigned use cleanup() to clear it first!! :P");
        fflush(stdout);
        return -1;//page already assigned
    }
}

int cleanup()
{
    //will have to traverse the list to dealloc each node
    struct chunk* current_chunk = paper_point->start_chunk;
    struct chunk* prev_chunk = 0x0;
    while(current_chunk!=0x0)
    {
        if (prev_chunk!=0x0)
        {
            //free the previous chunk
            free(prev_chunk);
        }
        prev_chunk=current_chunk;
        current_chunk=current_chunk->next_chunk;
    }
    if (munmap(paper_point->base_ptr, PAGESIZE)<0)
    {
        perror("munmap err: ");
        fflush(stdout);
        return errno;
    }
    free(paper_point);
    hasPage=false;
    return 0;
}

char* alloc(int requested_size)
{
    if (requested_size>paper_point->size_unallocated) 
    {
        printf("requested size > space left!!\n");
        return 0x0;
    }
    if (hasPage==false)
    {
        printf("no page has been allocated!!\n");
        return 0x0;
    }
    if (requested_size%MINALLOC!=0)
    {
        printf("pls, request a multiple of %d :)\n", MINALLOC);
        return 0x0;
    }
    struct chunk* this_chunk;
    this_chunk=paper_point->start_chunk;
    bool found=false;
    while (this_chunk!=nullptr && found==false)
    {
        if (this_chunk->inuse==false && this_chunk->size>=requested_size)
        {
            found=true;
            //do the nan-chunk cuts
            //i'll start filling the data from the start, i.e. after the split the second chunk will not be in use
            struct chunk* old_next_chunk=this_chunk->next_chunk;
            //get memory for the node
            if (this_chunk->size!=requested_size)
            {
            this_chunk->next_chunk=(chunk*)malloc(sizeof(struct chunk));
            //now configure both the chunks
            this_chunk->next_chunk->size=this_chunk->size-requested_size;
            this_chunk->size=requested_size;
            this_chunk->next_chunk->inuse=false;
            this_chunk->inuse=true;
            this_chunk->next_chunk->start_offset=this_chunk->start_offset+requested_size;
            this_chunk->next_chunk->next_chunk=old_next_chunk;
            }
            else
            {
                this_chunk->inuse=true;
            }
            //I think the split is ready to be served
        }   
        else
        {
            this_chunk=this_chunk->next_chunk;
        }
    }
    if (found==true)
    {
        fflush(stdout);
        return (char*)(this_chunk->start_offset+(char*)paper_point->base_ptr);
    }
    else
    {
        printf("too much fragmentation!!");
        fflush(stdout);
        return 0x0;
    }
}

void _chunkstat() // bonus from himanshu <3
{
    if (hasPage==false)
    {
        printf("no page to chunkstat\n");
        return;
    }
    struct chunk* this_chunk=paper_point->start_chunk;
    printf("------------------------------>\n");
    while(this_chunk!=0x0)
    {
        printf("size=%d, inuse=%d, offset=%d\n", this_chunk->size, this_chunk->inuse, this_chunk->start_offset);
        this_chunk=this_chunk->next_chunk;
    }
    printf("------------------------------>\n");
    return;
}