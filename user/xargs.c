#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXSZ 512

int
main(int argc, char *argv[])
{
  char c,buf_argv[MAXSZ],*begin,*end;
  int Xargc;
  char *Xargv[MAXARG];

  if(argc < 2){
    fprintf(2,"xargs: param error!\n");
    exit(1);
  }

  for (int i = 0; i < argc-1; i++)
    Xargv[i]=argv[i+1];
  Xargc=argc-1;
  begin=buf_argv;
  end=buf_argv;
  while (read(0,&c,1)==1)
  {
    if(c==' '){
      *end++='\0';
      Xargv[Xargc]=begin;
      Xargc++;
      begin=end;
      // memcpy(Xargv[Xargc],buf_argv,strlen(buf_argv));
      // Xargc++;
    }
    else if(c=='\n'){
      if(*begin!=0){
        *end++='\0';
        Xargv[Xargc]=begin;
        Xargc++;
        begin=end;
      }
      // memcpy(Xargv[Xargc],buf_argv,strlen(buf_argv));
      // Xargc++;
      if(fork()==0){
        exec(argv[1],Xargv);
      }else{
        wait(0);
        // 清空Xargc中一行的参数
        for (int i = argc-1; i < Xargc; i++)
          Xargv[i]=0;
        Xargc=argc-1;
      }
    }
    else
      *end++=c;
  }
  
  exit(0);
}
