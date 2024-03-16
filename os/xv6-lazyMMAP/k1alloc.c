// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct ref {
  struct ref *next;
  struct run *page;  
  int ref_count;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  struct ref *refs;
  int num_free_pages;
  int num_allocd;
} kmem;

int inited=0;
// below implementation is same as
// int returnable=0; 
// struct run* iter=kmem.freelist;
// while(iter)
// {
//   returnable++;
//   iter=iter->next;
// }
// return returnable;
//check with
// cprintf("returnable=%d, kmem.num_free_pages=%d\n", returnable, kmem.num_free_pages);

int gettingNumFreePages(void){
  struct ref* iter=kmem.refs;
  int count=0;
  while (iter)
  {
    count++;
    iter=iter->next;
  }
  cprintf("refs: %d, allocd: %d\n", count, kmem.num_allocd);
  return kmem.num_free_pages;
}

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  kmem.use_lock = 0;
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}

static void ref_update(char* this_run, int increment)
{
  //safest way to bypass malloc/ dangling reference like problems~ static
  struct run *r = (struct run*)this_run;
  struct ref* iter=kmem.refs; 
  struct ref* prev=0x0;
  int found=0;
  while (iter && found==0 && 0)//!! 
  {
    if (iter->page==(struct run*)this_run)
    {
      found=1;
      iter->ref_count=iter->ref_count+increment;
    }
    else
    {
      prev=iter;
      iter=iter->next;
    }
  }
  if (found)
  {
      if (iter->ref_count==0)
      {
        //delete this from refrences and add to freelist
        inited=1;

        if (prev=0x0)
        {
            kmem.refs=iter->next;
        }
        else
        {
            prev->next=iter->next;
        }
        r->next = kmem.freelist;
        kmem.freelist = r;
        kmem.num_free_pages++;
        // kmem.num_allocd--;
      }
      if (iter->ref_count<0)
      {
        //this should never happen
        panic("ref_update1");
      }
  }
  else
  {
      if (increment>0)
      {
        //rem from free list
        //add to reflist
        //make a new ref object

        struct ref new_ref;
        new_ref.ref_count=increment;
        new_ref.page=r;
        new_ref.next=kmem.refs;
        kmem.refs=&new_ref;

        kmem.freelist = r->next;
        kmem.num_free_pages--;
        kmem.num_allocd++;
      }
      else
      {
        if (inited) panic("ref_UpdateAssumptionViolated!");
        //the initial insertions to kmem.freelist
        // panic("ref_update2");
        //control never reaches here after booting into shell
        r->next = kmem.freelist;
        kmem.freelist = r;
        kmem.num_free_pages++;//for initalizing free pages
      }
  }
  //do not return before this line
}

//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);
  // if (inited==0)
  // {
  //   r = (struct run*)v;
  //   r->next = kmem.freelist;
  //   kmem.freelist = r;
  //   kmem.num_free_pages++;
  // }else{
    ref_update(v, -1);
  // }
  if(kmem.use_lock)
    release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  if(kmem.use_lock)
    acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
  {
    // kmem.freelist = r->next;
    // kmem.num_free_pages--;
    ref_update((char*)r, 1);
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}