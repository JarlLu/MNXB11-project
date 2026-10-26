
CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include
ROOTINCLUDES := $(shell root-config --incdir)
CXXFLAGS := $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES) $(shell root-config --cflags)
LDFLAGS := $(shell root-config --libs)

.PHONY: all clean

all: main


main: main.cxx src/DataPointDict.cxx src/DataPoint.o src/Csv2TTree.o src/Dat2TTree.o src/climate_change.o src/monthly_coldest.o src/Weekday.o | DataPointDict_rdict.pcm
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.cxx
	$(CXX) $(CXXFLAGS) $^ -c -o $@

src/DataPointDict.cxx: include/DataPoint.h include/LinkDef.h
	rootcling -f $@ -I$(ROOTINCLUDES) -Iinclude include/DataPoint.h include/LinkDef.h

DataPointDict_rdict.pcm: src/DataPointDict.cxx
	cp src/DataPointDict_rdict.pcm $@

clean:
	rm -vf main src/*.o src/DataPointDict.cxx src/DataPointDict_rdict.pcm DataPointDict_rdict.pcm 
