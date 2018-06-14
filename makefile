PCNAME	:= $(shell uname -n)
OSNAME	:= $(shell uname)
ifeq ($(PCNAME), nova)
	CC	:= g++-5.3.0
else ifeq ($(OSNAME), Darwin)
	CC	:= g++-8
else
	CC	:= g++
endif

CFLAGS		:= -std=c++14 -Wall -Wextra -Werror -pedantic-errors -fPIC -DNDEBUG -g

EXE_TARGET	:= ex3
EXE_FLAGS	:= -pthread -rdynamic -ldl -lstdc++fs -g
EXE_OBJS	:= main.o TournamentManager.o GameManager.o Piece.o

LIB_TARGET	:= RSPPlayer_203521984.so
LIB_FLAGS	:= -shared -g
LIB_OBJS	:= AutoPlayerAlgorithm.o

.PHONY: clean

all: rps_tournament rps_lib

rps_tournament: $(EXE_TARGET)

rps_lib: $(LIB_TARGET)

$(EXE_TARGET): $(EXE_OBJS)
	$(CC) $(EXE_OBJS) -o $@ $(EXE_FLAGS)

$(LIB_TARGET): $(LIB_OBJS)
	$(CC) $(LIB_OBJS) -o $@ $(LIB_FLAGS)

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

$(OBJS): %.o : %.cpp
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(EXE_TARGET) $(LIB_TARGET)

-include $(DEPS)