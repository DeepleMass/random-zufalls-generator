#!/bin/bash

rm  /usr/lib/libstosegen/libstosegen.so && rmdir /usr/lib/libstosegen/

cd /usr/include/stosegen/ && rm -R $(ls) && cd .. && rmdir /usr/include/stosegen
