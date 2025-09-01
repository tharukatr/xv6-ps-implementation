#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if(t == SBRK_EAGER || n < 0) {
    if(growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if(addr + n < addr)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct proc proc[NPROC];
extern struct spinlock wait_lock;

uint64
sys_ps(void)
{
  int numprocs = 0;
  acquire(&wait_lock);
  for (int i = 0; i < NPROC; i++)
  {
    struct proc *p = &proc[i];
    if (p->state != UNUSED)
    {
      numprocs++;
      char *state = "";
      switch (p->state)
      {
        case RUNNABLE: state = "runnable"; break;
        case ZOMBIE: state = "zombie"; break;
        case RUNNING: state = "running"; break;
        case SLEEPING: state = "sleeping"; break;
        default: break;
      }
      printf("%d %d %s %s %ld\n", p->pid, p->parent ? p->parent->pid : 0, state, p->name, p->sz);
    }
  }
  release(&wait_lock);
  printf("There are a total of %d processes in the system", numprocs);
  return 0;
}

uint64
sys_psr(void)
{
  acquire (&wait_lock);
  int numprocs = 0;

  for (int i = 0; i < NPROC; i++)
  { 
  struct proc *p = &proc[i];

  if (p->state == RUNNING)
  {
    printf("%d %d running %s %ld\n", p->pid, p-> parent ? p->parent->pid : 0, p->name, p->sz);
    numprocs++;
  }
}
  release(&wait_lock);
  return 0;
}