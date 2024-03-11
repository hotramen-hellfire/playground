#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

//granularity of memory to mmap from OS
#define PAGESIZE 4096 

//minimum allocation size
#define MINALLOC 256

//struct definitions
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

struct paper** notebook;//array of paper*s
bool has_book=false;

// function declarations to support
void init_alloc(void);
char *alloc(int);
void dealloc(char *);
void cleanup(void);

void init_alloc()
{
    if (has_book==true)
    {
        printf("a book exists, use cleanup() to clear!!\n");
        return -1;
    }
    //make a book
}


