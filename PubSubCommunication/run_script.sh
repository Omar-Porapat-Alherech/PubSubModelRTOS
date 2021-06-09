#!/bin/bash
make
exit_code=$?
if [ $exit_code -ne 0 ]; then
  echo "Error compiling/linking/object file and module";
  exit 1
  fi
terms=(urxvt rxvt xterm gnome-terminal)
for t in ${terms[*]}
do
    if [ $(command -v $t) ]
    then
        detected_term=$t
        break
    fi
done
gcc usersspace.c -o output
echo $detected_term "found"
for (( c=1; c<=3; c++ ))
do
   detected_term -- e ./output &
done
exit_code=$?
if [ $exit_code -ne 0 ]; then
  echo "Error C file";
  exit 1
  fi
sudo insmod pubsub.ko
#echo "=====================     Cleanup    ======================="
#sudo rmmod pubsub
#make clean
#sleep .5
##echo "===================== Output of Dmsg ======================="
##dmesg --level=info | tail -10
