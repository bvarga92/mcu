#!/bin/bash

stty -F /dev/ttyACM0 9600 cs8 -cstopb -parenb

sendCommand(){
	local cmd="\$$1 $2 $3 $4 $5|"
	local i;
	for((i=0;i<${#cmd};i++)); do
		echo "${cmd:$i:1}" > /dev/ttyACM0
	done
}

sendCommand 0000 000 1 0 0 #bal index
sleep 3
sendCommand 0000 000 2 1 0 #jobb index
sleep 3
sendCommand 0000 000 0 2 1 #fenyszoro
sleep 2
sendCommand 0000 000 0 0 2 #fenyszoro ki

for((i=0;i<=120;i=i+4)); do
	spd=$(printf "%03d" "$i")
	sendCommand 0000 $spd 0 0 0 #sebesseg lassan 120-ra
	sleep .1
done

for((i=0;i<=6000;i=i+200)); do
	rpm=$(printf "%04d" "$i")
	sendCommand $rpm 120 0 0 0 #fordulatszam lassan 6000-re
	sleep .1
done

sleep 2
sendCommand 0000 000 0 0 0 #mindent nullazunk

