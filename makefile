EXEC	:= ex3
CC		:= g++
CFLAGS	:= -std=c++14 -Wall -Wextra -Werror -pedantic-errors -pthread -ldl -lstdc++fs -DNDEBUG -g
SRCDIR	:= .
OBJDIR	:= .
BINDIR	:= .

# detecting all src files
SOURCES	= $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET	= $(BINDIR)/$(EXEC)

OSNAME	:= $(shell uname -n)
ifeq ($(OSNAME), nova)
	CC	:= g++-5.3.0
else
	CC	:= g++
endif

.PHONY: clean zip

default : all

all: rps_tournament

rps_tournament: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS)

zip:
	zip hw2_203521984_203774849 *.cpp *.h makefile *.txt