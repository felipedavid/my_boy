#!/bin/bash
[[ ! -d "bin" ]] && mkdir bin
gcc *.c -o bin/my_boy -g -Og
