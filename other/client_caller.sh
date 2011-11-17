#!/bin/bash

./client 127.0.0.1 4455 nekem &
./client 127.0.0.1 4455 nem  &
./client 127.0.0.1 4455 hasznal &
./client 127.0.0.1 4455 a &
./client 127.0.0.1 4455 szo &
./client 127.0.0.1 4455 en &
./client 127.0.0.1 4455 nem &
./client 127.0.0.1 4455 tudom &
./client 127.0.0.1 4455 mi &
./client 127.0.0.1 4455 a &
./client 127.0.0.1 4455 jo &

sleep 2
./client 127.0.0.1 4455 "en hiaba megyek fejjel" &
./client 127.0.0.1 4455 "a falnak, nekem nem hasznal"  &
./client 127.0.0.1 4455 "nem hasznal a szo" &
