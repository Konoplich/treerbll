#!/bin/bash
rm ./ok
gcc main.c -o ok -L. -ltreerbb -Wl,-rpath,. -I.
