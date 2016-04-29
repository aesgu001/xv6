#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

void
sys_exit(void)
{
  int *ip = 0;
  if (argint(0, ip) < 0)
    exit(1);
  exit(*ip);
}

int
sys_wait(void)
{
  char *p = 0;
  if (argptr(0, &p, 4) < 0)
    return -1;
  int *status = (int*)p;
  return wait(status);
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
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_waitpid(void)
{
  int *ip = 0;
  char *p = 0;
  if (argint(0, ip) < 0 || argptr(1, &p, 4) < 0)
    return -1;
  int *status = (int*)p;
  return waitpid(*ip, status, 0);
}

void
sys_change_priority(void)
{
  int *ip = 0;
  if (argint(0, ip) < 0)
    change_priority(0);
  else
    change_priority(*ip);
}
