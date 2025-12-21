#!/bin/sh

clear
ASAN_OPTIONS=detect_leaks=0 ./build/compiler tests/prg.txt