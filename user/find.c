#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
primes(int pipedata[2])
{
  int p;
  int neighbor[2];

  pipe(neighbor);
  close(pipedata[1]);
  if(read(pipedata[0],&p,sizeof(int))==sizeof(int)){
    fprintf(2,"prime %d\n",p);
    int n;
    while (read(pipedata[0],&n,sizeof(int))==sizeof(int))
    {
      if(n%p!=0){
        write(neighbor[1],&n,sizeof(int));
      }
    }
    if(fork() == 0){
      primes(neighbor);
      close(neighbor[0]);
      close(neighbor[1]);
    }
    else{
      close(neighbor[0]);
      close(neighbor[1]);
      wait(0);
    }
  }
}
int
main(int argc, char *argv[])
{
  int pipedata[2];
  pipe(pipedata);
  for(int i=2;i<=35;i++)
    write(pipedata[1],&i,sizeof(int));
  if(fork() == 0){
    primes(pipedata);
    close(pipedata[0]);
    close(pipedata[1]);
  }
  else{
    close(pipedata[0]);
    close(pipedata[1]);
    wait(0);
  }
  exit(0);
}
