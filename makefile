CC = g++
CFLG = -std=c++14 -g -Wall
INCS =
LFLG =
LIBS = -lglfw
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
EXEC = smoke

.PHONY: all clean run

all: $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LFLG) $(LIBS)

-include $(DEPS)

%.o: %.cpp
	$(CC) $(CFLG) $(INCS) -c  $< -o $@
	@$(CC) $(CFLG) $(INCS) -MM $< > $*.d

clean:
	rm -f $(OBJS) $(DEPS) $(EXEC)

run:
	./smoke
