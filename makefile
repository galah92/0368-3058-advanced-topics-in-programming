EXEC	:= ex2
CC		:= g++
CFLAGS	:= -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -ggdb
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

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

zip:
	zip hw2_203521984_203774849 *.cpp *.h makefile *.txt