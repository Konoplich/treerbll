#!/bin/bash
F=knowhow.c
I=./test_interface/interface/test_public/public/test_protected/protected/test_private/private/test_knowhow/knowhow
<<<<<<< HEAD
rm ./libtreerbll.so
gcc -shared -o libtreerbll.so -fPIC ${F} -I${I}
cp libtreerbll.so ..
cp libtreerbll.h ..
=======
rm ./libtreerbb.so
gcc -shared -o libtreerbb.so -fPIC ${F} -I${I}
cp libtreerbb.so ..
cp libtreerbb.h ..
>>>>>>> c2b30911f5674b6830f4dcc37247ff9f9509902b
