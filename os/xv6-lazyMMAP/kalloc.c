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

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  int num_free_pages;
} kmem;

struct spinlock reflock;
int refs_count[PHYSTOP/PGSIZE];

int gettingNumFreePages(void){
  return kmem.num_free_pages;
}

// Initialization happens in two phases.
// 1. main() calls kinit1() while, still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  // initlock(&reflock, "ref");//2nd arguement is the name of the lock// will use kmemlock
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
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE) //vend is PHYSTOP
  {
    refs_count[(uint)p/PGSIZE]=1;
    kfree(p);
  }  
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
  // we will have to andle the edge when 2 or more threrads call kfree at the same time, nothing equality exact with threads
  
  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);

  if (refs_count[(uint)v/PGSIZE]<=1)
  {
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
    kmem.num_free_pages++;
  }
  else
  {
    refs_count[(uint)v/PGSIZE]-=1;
  }
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
    kmem.freelist = r->next;
    kmem.num_free_pages--;
    refs_count[(uint)r/PGSIZE]=1;
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

int get_ref(uint pa)
{
  return refs_count[pa/PGSIZE];
}

void update_ref(uint pa, int increment)
{
  if(kmem.use_lock)
    acquire(&kmem.lock);
    refs_count[pa/PGSIZE]+=increment;
    if (refs_count[pa/PGSIZE]<1) kfree((char*)pa);
    // cprintf("moo\n");
  if(kmem.use_lock)
    release(&kmem.lock);
    return;
}