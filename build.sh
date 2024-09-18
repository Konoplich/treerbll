#!/bin/bash
rm ./ok
gcc main.c -o ok -L. -ltreerbll -Wl,-rpath,. -I.
