
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


main: main.cxx src/DataPoint.cxx src/Csv2TTree.cxx src/Dat2TTree.cxx src/climate_change.cxx src/monthly_coldest.cxx src/allcities.cxx src/DataPointDict.cxx | DataPointDict_rdict.pcm
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

src/DataPointDict.cxx: include/DataPoint.h include/LinkDef.h
	rootcling -f $@ -I$(ROOTINCLUDES) -Iinclude include/DataPoint.h include/LinkDef.h

DataPointDict_rdict.pcm: src/DataPointDict.cxx
	cp src/DataPointDict_rdict.pcm $@

clean:
	rm -f main src/*.o src/DataPointDict.cxx src/DataPointDict_rdict.pcm DataPointDict_rdict.pcm src/DataPointDict.rootmap