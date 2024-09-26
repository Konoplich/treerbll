#!/bin/bash
F=knowhow.c
I=./test_interface/interface/test_public/public/test_protected/protected/test_private/private/test_knowhow/knowhow
rm ./libtreerbll.so
gcc -shared -o libtreerbll.so -fPIC ${F} -I${I}
cp libtreerbll.so ..
cp libtreerbll.h ..
