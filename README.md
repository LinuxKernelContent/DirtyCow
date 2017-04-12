# DirtyCow - Exploit

Below code takes advantage of a know vulnerability [Dirty COW (CVE-2016-5195)] to escalate privileges and get root access.

Before running, uncomment sc for your platform within main.c.

Compilation:
gcc main.c -o dirtycow -lpthread

