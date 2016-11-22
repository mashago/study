how to install:
cd curl-xxxx
./configure
make
make test
sudo make install

include:
/Usr/local/include/curl

lib:
osx:
/Usr/local/lib/libcurl.a
/Usr/local/lib/libcurl.dylib

how to use:
in .cpp:
#include <curl/curl.h>
in Makefile:
g++ -o test test.cpp -lcurl

https://curl.haxx.se/download.html
http://www.cnblogs.com/moodlxs/archive/2012/10/15/2724318.html
http://dahu.co.uk/blog/?p=77
