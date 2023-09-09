####################################################################################
# Precompiled Headers In Practice
#
# https://stackoverflow.com/questions/53969396/precompiled-headers-not-used-by-gcc-when-building-with-a-makefile
####################################################################################
OUT_DIR = build

CXX = g++
CXXFLAGS = -O2 -H -Wall -std=c++14 -Wno-ignored-attributes
CXXFLAGS += -I /usr/include/eigen3 -I /usr/include -I $(OUT_DIR)
LINKFLAGS = -L /usr/lib/x86_64-linux-gnu/ -lgtest -lgtest_main
TIME = /usr/bin/time --format "buildtime: real=%e user=%U sys=%S [ %C ]"

OBJ = test_geometry.o #more objects here eventually I would think!

PCH_SRC = precomp.h
PCH_HEADERS = 
PCH_OUT = $(OUT_DIR)/precomp.h.gch


#@ $(eval LOGFILE=$(shell echo main-`date +%Y%m%dT%H%M%S`.log))
# LOGFILE = $(shell echo main-`date +%Y%m%dT%H%M%S`.log)
#LOGFILE = main-`date +%Y%m%dT%H%M%S`.log
LOGFILE = build.log

main: $(OBJ)
	@ $(TIME) $(CXX) $(CXXFLAGS) -o $@ $^ $(LINKFLAGS) >> $(LOGFILE) 2>&1

# Compiles your PCH
$(PCH_OUT): $(PCH_SRC) $(PCH_HEADERS)
	@ mkdir -p $(OUT_DIR)
	@ $(TIME) $(CXX) $(CXXFLAGS) -o $@ $<  >> $(LOGFILE) 2>&1

# the -include flag instructs the compiler to act as if lib.hpp
# were the first header in every source file
%.o: %.cpp $(PCH_OUT)
	@ $(TIME) $(CXX) $(CXXFLAGS) -include $(OUT_DIR)/$(PCH_SRC) -c -o $@ $< >> $(LOGFILE)  2>&1

clean:
	@ rm -rf main $(PCH_OUT) *.o $(OUT_DIR)
	@ rm -f $(LOGFILE)
