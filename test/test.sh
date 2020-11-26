#!/bin/bash

echo "Pruebas secuenciales Threads: 1  Ciclos: 1"
./bin/client localhost 6785 img1.png 1 1 &
./bin/client localhost 6786 img1.png 1 1 &
./bin/client localhost 6787 img1.png 1 1 &

wait

echo "Pruebas secuenciales Threads: 1  Ciclos: 10"
./bin/client localhost 6785 img1.png 1 10 &
./bin/client localhost 6786 img1.png 1 10 &
./bin/client localhost 6787 img1.png 1 10 &

wait

echo "Pruebas secuenciales Threads: 1  Ciclos: 50"
./bin/client localhost 6785 img1.png 1 50 &
./bin/client localhost 6786 img1.png 1 50 &
./bin/client localhost 6787 img1.png 1 50 &

wait

echo "Pruebas secuenciales Threads: 1  Ciclos: 100"
./bin/client localhost 6785 img1.png 1 100 &
./bin/client localhost 6786 img1.png 1 100 &
./bin/client localhost 6787 img1.png 1 100 &

wait
# ##########################################################

echo "Pruebas Paralelas Threads: 4  Ciclos: 2"
./bin/client localhost 6785 img1.png 4 2 &
./bin/client localhost 6786 img1.png 4 2 &
./bin/client localhost 6787 img1.png 4 2 &

wait

echo "Pruebas Paralelas Threads: 4  Ciclos: 10"
./bin/client localhost 6785 img1.png 4 10 &
./bin/client localhost 6786 img1.png 4 10 &
./bin/client localhost 6787 img1.png 4 10 &

wait

echo "Pruebas Paralelas Threads: 4  Ciclos: 20"
./bin/client localhost 6785 img1.png 4 20 &
./bin/client localhost 6786 img1.png 4 20 &
./bin/client localhost 6787 img1.png 4 20 &

wait