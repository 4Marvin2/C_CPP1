#!/usr/bin/env bash

g++ -std=c++11 file_generator.cpp -o generator
./generator size_10 10
./generator size_23 23
./generator size_123 123
./generator size_200 10
./generator size_555 555
./generator size_666 666
./generator size_1333 1333
./generator size_1777 1777
./generator size_5000 5000
./generator size_10000 10000

./generator size_20 20
./generator size_30 30
./generator size_40 40
./generator size_50 50
./generator size_60 60