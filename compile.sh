#
#    For the Sake of Humanity
#    We Write the Following Code
#    ...
#

#!/bin/bash
set -x

# gvef: compute the runs distribution according to specification
c++ -Wall -Wfatal-errors -g -std=c++11 src/specifications/groundSpecification.cpp shell/specifications/shellGroundSpecification.cpp -o bin/gvef

# kvef: get the runs cumulated specifiations according to specifications
c++ -Wall -Wfatal-errors -g -std=c++11 src/specifications/groundSpecification.cpp shell/specifications/shellCumulatedSpecification.cpp -o bin/kfev

# gvvef: get the runs ground distribution
c++ -Wall -Wfatal-errors -g -std=c++11 src/evaluation/groundDistribution.cpp shell/evaluation/shellGroundDistribution.cpp -o bin/gvvef

# kvvef: get the runs cumulated distribution
c++ -Wall -Wfatal-errors -g -std=c++11 src/evaluation/groundDistribution.cpp shell/evaluation/shellCumulatedDistribution.cpp -o bin/kvvef

# etef:  run the exponential driver (core function). this is the actuall stochastic sequential generator
c++ -Wall -Wfatal-errors -g -std=c++11 src/driver/driver.cpp shell/driver/shellDriver.cpp src/specifications/groundSpecification.cpp -o bin/etef

# compile all into a shared lib
c++ -Wall -Wfatal-errors -g -std=c++11  src/driver/driver.cpp src/evaluation/groundDistribution.cpp \
    src/specifications/groundSpecification.cpp src/tools/binToFloat.cpp src/tools/binToInteger.cpp \
    -shared -fPIC -o lib/libstosegen.so

# compute helper for conversion in differnt integer basis (binary, decimal, hexadec)
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBinToBool.cpp -o bin/bintobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBooltodec.cpp -o bin/booltodec
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBooltohex.cpp -o bin/booltohex
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellDectobool.cpp -o bin/dectobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellDectohex.cpp -o bin/dectohex
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellHextobool.cpp -o bin/hextobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellHextodec.cpp -o bin/hextodec

# test suite for the stochastic sequential generator
# c++ -Wall -Wfatal-errors -g -std=c++11 test/test.cpp -o test
