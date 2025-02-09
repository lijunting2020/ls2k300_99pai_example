#!/bin/sh
trap  unexport_gpio exit

unexport_gpio () {
    echo 88 > /sys/class/gpio/unexport
    echo 89 > /sys/class/gpio/unexport
    echo 72 > /sys/class/gpio/unexport
}

export_gpio () {
    echo 88 > /sys/class/gpio/export
    echo "out" > /sys/class/gpio/gpio88/direction
    echo 89 > /sys/class/gpio/export
    echo "out" > /sys/class/gpio/gpio89/direction 
    echo 72 > /sys/class/gpio/export
    echo "out" > /sys/class/gpio/gpio72/direction 
}

export_gpio

t=10
while [ $t -gt 0 ]
do
  echo 1 > /sys/class/gpio/gpio72/value
  sleep 8
  echo 0 > /sys/class/gpio/gpio72/value
  echo 1 > /sys/class/gpio/gpio89/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio89/value
  sleep 1
  echo 1 > /sys/class/gpio/gpio89/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio89/value
  sleep 1
  echo 1 > /sys/class/gpio/gpio89/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio89/value
  echo 1 > /sys/class/gpio/gpio88/value
  sleep 5
  echo 0 > /sys/class/gpio/gpio88/value
  sleep 1
  echo 1 > /sys/class/gpio/gpio88/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio88/value
  sleep 1
  echo 0 > /sys/class/gpio/gpio88/value  
done

