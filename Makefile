CC=gcc
CFLAGS=--std=c99 -Wextra -g
vpath %.c src/
vpath %.h src/

# --------------------------------------------------------------------
.PHONY: all
all: test
.PHONY: test
test: test_findkbd
# --------------------------------------------------------------------
.PHONY: clean
clean:
	-rm *.o

# --------------------------------------------------------------------
# MAIN PROGRAMS
test_findkbd: test_findkbd.c findkbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
