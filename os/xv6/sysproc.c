#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_hello(void)
{
  begin_op();
  cprintf("hello\n");
  end_op();
  return 0;
}

int sys_helloYou(void)
{
  char *name;
  begin_op();
  argstr(0, &name);
  cprintf(name);
  cprintf("\n");
  end_op();
  return 0;
}

int sys_getMaxPID(void)
{
  begin_op();
  int maxpid = getMaxPid();
  end_op();
  return maxpid;
}

int sys_getNumProc(void)
{
  begin_op();
  int numprocs = getNumProc();
  end_op();
  return numprocs;
}

int sys_getProcInfo(void)
{
  begin_op();
  int pid;
  argint(0, &pid);
  struct processInfo *pinfo;
  argint(1, (int *)&pinfo);
  int ret = getProcInfoSrc(pid, pinfo);
  end_op();
  return ret;
}

int sys_setprio(void)
{
  int n;
  argint(0, &n);
  struct proc *curproc = myproc();
  begin_op();
  curproc->priority = n;
  end_op();
  return 0;
}
int sys_getprio(void)
{
  int n;
  struct proc *curproc = myproc();
  begin_op();
  n = curproc->priority;
  end_op();
  return n;
}

int sys_welcomeFunction(void)
{
  uint parsed;
  struct proc *curproc = myproc();
  argint(0, &parsed);
  uint func_pointer = (uint)parsed;
  // cprintf("\nparsed: %d, \npointer: %d\n", parsed, func_pointer);
  begin_op();
  curproc->hasWelcome = 1;
  curproc->welcomeFunction = func_pointer;
  end_op();
  return 0;
}

int sys_welcomeDone(void)
{
  panic("you were a good friend!!\n");
  struct proc *curproc = myproc();
  begin_op();
  // if (curproc->returnFunction)
  if (curproc->hasReturnFunction == 1)
    curproc->tf->eip = curproc->returnFunction;
  else
  {
    cprintf("(panik) Current Program didn't enter with a welcome fork!!\n"); // handle err
    return -1;
  }
  end_op();
  return 0;
}
