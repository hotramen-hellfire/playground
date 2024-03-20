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
  int numfreepages;
  int refcount[PHYSTOP/PGSIZE];
} kmem;

int getNumFreePages(void)
{
    if (kmem.use_lock) acquire(&kmem.lock);
    int ret = kmem.numfreepages;
    // int ret =0;
    if (kmem.use_lock) release(&kmem.lock);
    return ret;
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
    panic("kfree 1");

  // Fill with junk to catch dangling refs.

  if(kmem.use_lock)
    acquire(&kmem.lock);
  if (kmem.refcount[V2P(v)/PGSIZE]>0) kmem.refcount[V2P(v)/PGSIZE]--;
  if (kmem.refcount[V2P(v)/PGSIZE]==0)
  {
    memset(v, 1, PGSIZE);
    r = (struct run*)v;
    r->next = kmem.freelist;
    kmem.freelist = r;
    kmem.numfreepages++;
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
    kmem.numfreepages--;
    if (kmem.refcount[V2P((char*)r)/PGSIZE]==0)
    kmem.refcount[V2P((char*)r)/PGSIZE]=1;
    else panic("kalloc");
  }
  if(kmem.use_lock)
    release(&kmem.lock);
  return (char*)r;
}

void increase(uint pa)
{
  // if(pa % PGSIZE || v < end || pa>= PHYSTOP)
  //   panic("increase 1");
  if(kmem.use_lock)
    acquire(&kmem.lock);
  if (kmem.refcount[pa/PGSIZE]<=0) panic("increase 2");// 1 should have been handled in _handleTheCow itself;
  kmem.refcount[pa/PGSIZE]++;
  if(kmem.use_lock)
    release(&kmem.lock);
  return;
}

void decrease(uint pa)
{
  // if(pa % PGSIZE || v < end || pa>= PHYSTOP)
  //   panic("decrease 1");
  if(kmem.use_lock)
    acquire(&kmem.lock);
  if (kmem.refcount[pa/PGSIZE]<=1) panic("decrease 2");// 1 should have been handled in _handleTheCow itself;
  kmem.refcount[pa/PGSIZE]--;
  if(kmem.use_lock)
    release(&kmem.lock);
  return;
}

int get_ref(uint pa){
  return kmem.refcount[pa/PGSIZE];
}



