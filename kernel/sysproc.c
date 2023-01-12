#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

// Following code is added by Sarthak Gupta SXG200139
#include "pstat.h"
int getpinfo(struct pstat*); // declaring the function for getpinfo

// End of code added


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Following code is added by SARTHAK GUPTA SXG200139

int 
sys_settickets(void){
  int num_tickets = 0;

  if (argint(0, &num_tickets) < 0)
    return -1;
  
  if (num_tickets < 1)
    return -1;

  proc->tickets = num_tickets;

  return 0;
}

int 
sys_getpinfo(void){
  struct pstat *pinfo = NULL;

  if (argptr(0, (void *)&pinfo, sizeof(*pinfo)) < 0)
    return -1;
  
  if (pinfo == NULL)
    return -1;

  
  return getpinfo(pinfo);
}
// End of modified code