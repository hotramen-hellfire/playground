#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

//granularity of memory to mmap from OS
#define PAGESIZE 4096 
#define NUMPAGES 4 
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

int clean_page(struct paper* paper_point)
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
    return 0;
}

void cleanup()
{
    if (has_book==false)
    {
        printf("nothing to cleanup");
        return;
    }
    for (int i=0; i<NUMPAGES && notebook[i]!=0x0; i++)
    {
        int ex=clean_page(notebook[i]);
    }
    free(notebook);
    has_book=false;
    return;
}

int dealloc_from_page(char * ptr, struct paper* paper_point)
{
    if ((int)(ptr-(char*)paper_point->base_ptr)>PAGESIZE-1)
    {
        return 1;
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
    if (found==false) return 1;
    return 0;
}

void dealloc(char * ptr)
{
    bool deleted=false;
    if (has_book==false)
    {
        printf("no book exists!!\n");
        return;
    }
    for (int i=0; i<NUMPAGES && notebook[i]!=0x0 && deleted==false; i++)
    {
        int ex=dealloc_from_page(ptr, notebook[i]);
        if (ex==0) {deleted=true;}
    }
    if (deleted==false)
    {   
        printf("no chunk to deallocate!!\n");
    }
    return;// return silently
}

void init_alloc()
{
    if (has_book==true)
    {
        printf("a book alredy exists, use cleanup() to clear!!\n");
        return;
    }
    //make a book
    has_book=true;
    notebook=(paper**)malloc(NUMPAGES*sizeof(paper*));
    for (int i=0; i<NUMPAGES; i++)
    {
        notebook[i]=0x0;//null initialization
    }//do not allocate a page yet
    return;
}

char* alloc_to_page(int requested_size, struct paper* paper_point)
{
    if (requested_size>paper_point->size_unallocated) 
    {
        // printf("requested size > space left!!\n");
        return 0x0;
    }
    if (paper_point==0x0)
    {
        // printf("no page has been allocated!!\n");
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
        // printf("too much fragmentation!!");
        fflush(stdout);
        return 0x0;
    }
}



char* alloc(int requested_size)
{
    bool didAlloc=false;
    char* ptr;
    for (int i=0; i<NUMPAGES && didAlloc==false; i++)
    {
        if (notebook[i]==0x0)
        {
            //allocate new page if required
            notebook[i]=(paper*)malloc(sizeof(paper));
            //now init this new paper ds
            void *ptr = mmap(0, PAGESIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
            if (ptr==MAP_FAILED)
            {
                printf("(%d)", errno);
                perror("mmap failed: ");
                fflush(stdout);
                // return errno;
                return 0x0;
            }
            struct paper* paper_point=notebook[i];
            paper_point->start_chunk=(chunk*)malloc(sizeof(struct chunk));
            paper_point->start_chunk->size=PAGESIZE;
            paper_point->start_chunk->inuse=false;
            paper_point->start_chunk->next_chunk=0x0;
            paper_point->start_chunk->start_offset=0;
            paper_point->size_unallocated=PAGESIZE;
            paper_point->base_ptr=ptr;
            //paper/ page is configured
        }
            //check for a segment of requested size
            ptr=alloc_to_page(requested_size, notebook[i]);
            if (ptr!=nullptr) didAlloc=true;
    }

    if (didAlloc==true)
    {
        return ptr;
    }
    else
    {
        printf("too much frag!!\n");
        return 0x0;
    }
}

void _chunkstat(struct paper* paper_point) // bonus from himanshu <3
{
    struct chunk* this_chunk=paper_point->start_chunk;
    printf("--->\n");
    while(this_chunk!=0x0)
    {
        printf("size=%d, inuse=%d, offset=%d\n", this_chunk->size, this_chunk->inuse, this_chunk->start_offset);
        this_chunk=this_chunk->next_chunk;
    }
    printf("--->\n");
    return;
}

void _pagestat()
{
    if (has_book==false)
    {
        printf("no book to pagestat!!\n");
        return;
    }
    printf("---------------------->\n");
    for (int i=0; i<NUMPAGES; i++)
    {
        if (notebook[i]!=0) _chunkstat(notebook[i]);
        else break;
    }
    printf("---------------------->\n");
    return;
}


