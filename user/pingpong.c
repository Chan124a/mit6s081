#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[1];
  int parent_to_child[2];
  int child_to_parent[2];
  pipe(parent_to_child);
  pipe(child_to_parent);
  if(fork() == 0){
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    if(read(parent_to_child[0],buf,1)>0){
      fprintf(2,"%d: received ping\n",getpid());
      close(parent_to_child[0]);
      if(write(child_to_parent[1],"pong",1)!=1){
        fprintf(2,"write error\n");
        exit(1);
      }
      close(child_to_parent[1]);
    }
  }else{
    close(parent_to_child[0]);
    if(write(parent_to_child[1],"ping",1)!=1){
      fprintf(2,"write error\n");
      exit(1);
    }
    close(parent_to_child[1]);
    
    close(child_to_parent[1]);
    wait(0);
    if(read(child_to_parent[0],buf,1)>0){
      fprintf(2,"%d: received pong\n",getpid());
      close(child_to_parent[0]);
    }
  }
  exit(0);
}
