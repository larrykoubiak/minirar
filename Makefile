STATIC_LINKING := 0
AR             := ar

#detect platform
ifeq ($(platform),)
platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = mingw
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
endif
endif

#detect architecture
ifeq ($(shell uname -m),)
   arch = i686
else
   arch = ($(shell uname -m)
endif

#set extension and lib path

ifneq ($(filter $(platform),win mingw),)
EXT = .exe
else ifeq ($(platform), unix)
EXT = 
fpic = -fPIC
endif

CC = gcc
CXX = g++
INCFLAGS= -I.

CFLAGS += -Wall -std=c99 $(INCFLAGS)
CXXFLAGS += -Wall $(INCFLAGS)
#LDFLAGS += -shared

TARGET_NAME := minirar
EXECUTABLE = $(TARGET_NAME)$(EXT)

SOURCES_CXX := 

SOURCES_C    := stringtable.c \
				rar_headers.c \
				rar_time.c \
				main.c

OBJECTS := $(SOURCES_CXX:.cpp=.o) $(SOURCES_C:.c=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(fpic) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(fpic) -c -g -o $@ $<

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

all: $(EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

install:
	cp $(EXECUTABLE) ~/.config/retroarch/cores
	cp $(INFO) ~/.config/retroarch/cores

test:
	./minirar "test/Final Fantasy 6 [ff6].rsn"

test2:
	./minirar "test/ff7_psf.rar"

debug:
	gdb --args minirar "test/Final Fantasy 6 [ff6].rsn"
	
.PHONY: clean install test test2 debug
