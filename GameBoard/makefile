PCNAME	:= $(shell uname -n)
ifeq ($(PCNAME), nova)
	CC	:= g++-5.3.0
else
	CC	:= g++
endif

EXEC = ex4
CFLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

.PHONY: clean

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

$(OBJS): %.o : %.cpp
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(EXEC)