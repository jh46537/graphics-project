SRCD = src
INCD = include
OBJD = build

CC = g++
CFLG = -std=c++14 -g -Wall
INCS = -Iinclude
LFLG =
LIBS = -lglfw -ldl

SRCS = $(shell find $(SRCD) -name *.cpp -or -name *.c)
OBJS = $(patsubst $(SRCD)/%.c,$(OBJD)/%.o,$(patsubst $(SRCD)/%.cpp,$(OBJD)/%.o,$(SRCS)))
DEPS = $(OBJS:.o=.d)
EXEC = smoke

.PHONY: all $(OBJD) clean run

all: $(OBJD) $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LFLG) $(LIBS)

-include $(DEPS)

$(OBJD)/%.o: $(SRCD)/%.cpp

	$(CC) $(CFLG) $(INCS) -c  $< -o $@
	@$(CC) $(CFLG) $(INCS) -MM $< > $(OBJD)/$*.d
	@sed -i 's/^.*:/build\/&/' $(OBJD)/$*.d

$(OBJD)/%.o: $(SRCD)/%.c
	$(CC) $(CFLG) $(INCS) -c  $< -o $@
	@$(CC) $(CFLG) $(INCS) -MM $< > $(OBJD)/$*.d
	@sed -i 's/^.*:/build\/&/' $(OBJD)/$*.d

$(OBJD):
	mkdir -p $(OBJD)

clean:
	rm -rf $(OBJS) $(OBJSD) $(DEPS) $(EXEC)

run:
	./smoke
