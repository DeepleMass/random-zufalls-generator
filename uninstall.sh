#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#

#!/bin/bash

rm  /usr/lib/libstosegen/libstosegen.so && rmdir /usr/lib/libstosegen/

cd /usr/include/stosegen/ && rm -R $(ls) && cd .. && rmdir /usr/include/stosegen
