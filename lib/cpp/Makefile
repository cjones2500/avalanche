CFLAGS = -fPIC -g -DDEBUG
INCDIR = -I.
LDFLAGS = -L/usr/local/lib

CLANG := $(shell which clang)
ifneq ($(CLANG),)
    CC = clang++
    CFLAGS += 
    INCDIR += -I$(shell root-config --incdir)
else
    CC = g++
    CFLAGS += -shared
    INCDIR += -I$(ROOTSYS)/include
endif

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDFLAGS += -L$(shell root-config --libdir) -dynamiclib #-install_name /usr/local/lib/$(@F).so #-undefined dynamic_lookup
endif
ifeq ($(UNAME), Linux)
    LDFLAGS += -L$(ROOTSYS)/lib -rdynamic
endif

LIBS = -lzmq -lpthread -lcurl -ldl $(shell root-config --libs)

all: libavalanche

libavalanche:
	$(CC) -o $@.so server.cpp client.cpp stream.cpp json/json_reader.cpp json/json_writer.cpp json/json_value.cpp $(INCDIR) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

clean:
	-$(RM) *.o *.so

