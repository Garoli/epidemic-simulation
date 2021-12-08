#!/bin/bash
echo "Making project ..."
make clean
rm roundPipe timerPipe
rm evolution.txt
make
./bin/timer &
sleep 0.5
./bin/epidemic_sim &
sleep 0.5
./bin/press_agency &
sleep 0.5
./bin/citizen_manager
