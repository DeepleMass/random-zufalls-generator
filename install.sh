#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#

#!/bin/bash

## we just copy the couple of files at the canonical places

mkdir -p /usr/lib/libstosegen/ && cp lib/libstosegen.so /usr/lib/libstosegen/libstosegen.so

mkdir -p /usr/include/stosegen/ && cp -r header/* /usr/include/stosegen/