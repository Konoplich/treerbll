#!/bin/bash
rm ./ok
<<<<<<< HEAD
gcc main.c -o ok -L. -ltreerbll -Wl,-rpath,. -I.
=======
gcc main.c -o ok -L. -ltreerbb -Wl,-rpath,. -I.
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
