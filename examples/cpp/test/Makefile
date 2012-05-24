CFLAGS = -g
INCDIR = -I. -I$(AVALANCHE) -I$(shell root-config --incdir)
LDFLAGS = -L. -L$(AVALANCHE) -L/usr/local/lib -L$(shell root-config --libdir) 
LIBS = -lavalanche $(shell root-config --libs)

CLANG := $(shell which clang)
ifneq ($(CLANG),)
    CC = clang++
    CFLAGS += -std=c++11 -pipe #-stdlib=libc++
else
    CC = g++
    CFLAGS += -std=g++0x
endif

TARGETS := $(subst .cpp,, $(wildcard *.cpp))
ROOT_DICT := root_dict.o
DICT_OBJ := $(wildcard *.hh)

.PHONY: clean all .depend

all: $(TARGETS)

$(TARGETS):%:%.o

%.o: %.cpp
	@echo Compiling $@
	$(CC) -o $@ $(CFLAGS) $(INCDIR) -c $<

$(TARGETS): $(ROOT_DICT)
	@echo Linking $@
	$(CC) -o $@ $(LDFLAGS) $(LIBS) $@.o $(ROOT_DICT)

$(ROOT_DICT): $(ROOT_DICT:%.o=%.cc) $(ROOT_DICT:%.o=%.h)
	@echo Compiling $@
	$(CC) -o $@ $(CFLAGS) $(INCDIR) -c $<

$(ROOT_DICT:%.o=%.cc) $(ROOT_DICT:%.o=%.h): $(DICT_OBJ) LinkDef.h
	@echo Generating ROOT dictionary
	rootcint -f $(ROOT_DICT:%.o=%.cc) -c -p $(INCDIR) -D_REENTRANT $(DICT_OBJ) LinkDef.h

.depend:
	@echo Generating dependecies
	$(CC) -MM $(CFLAGS) $(INCDIR) *.cpp > $@

ifneq ($(MAKECMDGOALS), clean)
-include .depend
endif

clean:
	-$(RM) -r $(TARGETS) *.o *.dSYM .depend root_dict.cc root_dict.h


