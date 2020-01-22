#!/bin/bash

mkdir -p /usr/lib/libstosegen/ && cp lib/libstosegen.so /usr/lib/libstosegen/libstosegen.so

mkdir -p /usr/include/stosegen/ && cp -r header/* /usr/include/stosegen/