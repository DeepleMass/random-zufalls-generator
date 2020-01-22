#!/bin/bash
set -x

# Grundvorgabe Einwegfunktion
c++ -Wall -Wfatal-errors -g -std=c++11 src/specifications/groundSpecification.cpp shell/specifications/shellGroundSpecification.cpp -o bin/gvef

# Kumulierte Vorgabe  Einwegfunktion
c++ -Wall -Wfatal-errors -g -std=c++11 src/specifications/groundSpecification.cpp shell/specifications/shellCumulatedSpecification.cpp -o bin/kfev

# Grundverteilung  Einwegfunktion
c++ -Wall -Wfatal-errors -g -std=c++11 src/evaluation/groundDistribution.cpp shell/evaluation/shellGroundDistribution.cpp -o bin/gvvef

# Kumulierte Verteilung  Einwegfunktion
c++ -Wall -Wfatal-errors -g -std=c++11 src/evaluation/groundDistribution.cpp shell/evaluation/shellCumulatedDistribution.cpp -o bin/kvvef

# Exponentieller Treiber Einwegfunktion
c++ -Wall -Wfatal-errors -g -std=c++11 src/driver/driver.cpp shell/driver/shellDriver.cpp src/specifications/groundSpecification.cpp -o bin/etef

# compile the shared lib
c++ -Wall -Wfatal-errors -g -std=c++11  src/driver/driver.cpp src/evaluation/groundDistribution.cpp \
    src/specifications/groundSpecification.cpp src/tools/binToFloat.cpp src/tools/binToInteger.cpp \
    -shared -fPIC -o  lib/libstosegen.so

#Werkzeuge für die Bearbeitung der Eingaben
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBinToBool.cpp -o bin/bintobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBooltodec.cpp -o bin/booltodec
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellBooltohex.cpp -o bin/booltohex
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellDectobool.cpp -o bin/dectobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellDectohex.cpp -o bin/dectohex
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellHextobool.cpp -o bin/hextobool
c++ -Wall -Wfatal-errors -g -std=c++11 shell/tools/shellHextodec.cpp -o bin/hextodec
