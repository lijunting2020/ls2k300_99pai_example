#!/bin/sh
trap  'echo 83 > /sys/class/gpio/unexport ' exit

echo 83 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio83/direction
t=10
while [ $t -gt 0 ]
do
  echo 1 > /sys/class/gpio/gpio83/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio83/value
  sleep 1
done

