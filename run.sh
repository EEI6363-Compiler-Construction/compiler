#!/bin/sh

clear

FILE=${1:-prg}

ASAN_OPTIONS=detect_leaks=0 ./build/compiler "tests/$FILE.txt"
