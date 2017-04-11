# DirtyCow - Exploit

Below code takes advantage of a know vulnerability [Dirty COW (CVE-2016-5195)] to escalate privileges and get root access.


echo 0 > /proc/sys/vm/dirty_writeback_centisec
