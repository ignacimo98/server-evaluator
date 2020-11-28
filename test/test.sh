#!/bin/bash

echo "---Running Servers---"
./bin/server 6785 & 
srv=$!
./bin/servert 6786 &
srvt=$!
# ./bin/servertpool 6787 &
# srvtp=$!

echo "Pruebas secuenciales Threads: 1  Ciclos: 8"
./bin/client localhost 6785 img1.png 1 8 &
client1=$!
./bin/client localhost 6786 img1.png 1 8 &
client2=$!
./bin/client localhost 6787 img1.png 1 8 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 1  Ciclos: 16"
./bin/client localhost 6785 img1.png 1 16 &
client1=$!
./bin/client localhost 6786 img1.png 1 16 &
client2=$!
./bin/client localhost 6787 img1.png 1 16 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 1  Ciclos: 32"
./bin/client localhost 6785 img1.png 1 32 &
client1=$!
./bin/client localhost 6786 img1.png 1 32 &
client2=$!
./bin/client localhost 6787 img1.png 1 32 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 1  Ciclos: 64"
./bin/client localhost 6785 img1.png 1 64 &
client1=$!
./bin/client localhost 6786 img1.png 1 64 &
client2=$!
./bin/client localhost 6787 img1.png 1 64 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 1  Ciclos: 128"
./bin/client localhost 6785 img1.png 1 128 &
client1=$!
./bin/client localhost 6786 img1.png 1 128 &
client2=$!
./bin/client localhost 6787 img1.png 1 128 &
client3=$!

wait $client1 $client2 $client3


# # ##########################################################

echo "Pruebas secuenciales Threads: 2  Ciclos: 4"
./bin/client localhost 6785 img1.png 2 4 &
client1=$!
./bin/client localhost 6786 img1.png 2 4 &
client2=$!
./bin/client localhost 6787 img1.png 2 4 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 2  Ciclos: 8"
./bin/client localhost 6785 img1.png 2 8 &
client1=$!
./bin/client localhost 6786 img1.png 2 8 &
client2=$!
./bin/client localhost 6787 img1.png 2 8 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 2  Ciclos: 16"
./bin/client localhost 6785 img1.png 2 16 &
client1=$!
./bin/client localhost 6786 img1.png 2 16 &
client2=$!
./bin/client localhost 6787 img1.png 2 16 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 2  Ciclos: 32"
./bin/client localhost 6785 img1.png 2 32 &
client1=$!
./bin/client localhost 6786 img1.png 2 32 &
client2=$!
./bin/client localhost 6787 img1.png 2 32 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas secuenciales Threads: 2  Ciclos: 64"
./bin/client localhost 6785 img1.png 2 64 &
client1=$!
./bin/client localhost 6786 img1.png 2 64 &
client2=$!
./bin/client localhost 6787 img1.png 2 64 &
client3=$!

wait $client1 $client2 $client3


##############################################################################

echo "Pruebas Paralelas Threads: 4  Ciclos: 2"
./bin/client localhost 6785 img1.png 4 2 &
client1=$!
./bin/client localhost 6786 img1.png 4 2 &
client2=$!
./bin/client localhost 6787 img1.png 4 2 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas Paralelas Threads: 4  Ciclos: 10"
./bin/client localhost 6785 img1.png 4 10 &
client1=$!
./bin/client localhost 6786 img1.png 4 10 &
client2=$!
./bin/client localhost 6787 img1.png 4 10 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas Paralelas Threads: 4  Ciclos: 20"
./bin/client localhost 6785 img1.png 4 20 &
client1=$!
./bin/client localhost 6786 img1.png 4 20 &
client2=$!
./bin/client localhost 6787 img1.png 4 20 &
client3=$!

wait $client1 $client2 $client3

# # ##########################################################

echo "Pruebas Paralelas Threads: 8  Ciclos: 2"
./bin/client localhost 6785 img1.png 4 2 &
client1=$!
./bin/client localhost 6786 img1.png 4 2 &
client2=$!
./bin/client localhost 6787 img1.png 4 2 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas Paralelas Threads: 8  Ciclos: 10"
./bin/client localhost 6785 img1.png 4 10 &
client1=$!
./bin/client localhost 6786 img1.png 4 10 &
client2=$!
./bin/client localhost 6787 img1.png 4 10 &
client3=$!

wait $client1 $client2 $client3

echo "Pruebas Paralelas Threads: 8  Ciclos: 20"
./bin/client localhost 6785 img1.png 8 20 &
client1=$!
./bin/client localhost 6786 img1.png 8 20 &
client2=$!
./bin/client localhost 6787 img1.png 8 20 &
client3=$!

wait $client1 $client2 $client3


echo "---Stopping Servers---"
kill $srv $srvt

echo "end"

# wait
