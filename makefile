EXEC	:= ex1
CC		:= g++
CFLAGS	:= -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -ggdb
SRCDIR	:= .
OBJDIR	:= .
BINDIR	:= .

# detecting all src files
SOURCES	= $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
TARGET	= $(BINDIR)/$(EXEC)

.PHONY: build clean

default : all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SDLLIB) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(SDLINC) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
