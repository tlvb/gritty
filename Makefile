CC=gcc
CFLAGS=--std=c99 -Wextra -g
vpath %.c src/
vpath %.h src/

# --------------------------------------------------------------------
.PHONY: all
all: main tests
.PHONY: main
main: gritty
.PHONY: tests
tests: test_findkbd test_read_and_translate
# --------------------------------------------------------------------
.PHONY: clean
clean:
	-rm *.o

# --------------------------------------------------------------------
# MAIN PROGRAMS
gritty: gritty.c kbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)

# --------------------------------------------------------------------
# TEST PROGRAMS
test_read_and_translate: test_read_and_translate.c kbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)
test_findkbd: test_findkbd.c kbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
