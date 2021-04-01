#!/usr/bin/env bash

g++ -std=c++11 file_generator.cpp -o generator
./generator size_50 50
./generator size_150 150
./generator size_2500 2500
./generator size_7500 7500
./generator size_15000 15000
./generator size_50000 50000
./generator size_150000 150000
./generator size_100000000 100000000