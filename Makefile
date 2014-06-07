CC=gcc
CFLAGS=--std=c99 -Wextra -g
vpath %.c src/
vpath %.h src/

# --------------------------------------------------------------------
.PHONY: all
all: test
.PHONY: test
test: test_findkbd test_read_and_translate
# --------------------------------------------------------------------
.PHONY: clean
clean:
	-rm *.o

# --------------------------------------------------------------------
# TEST PROGRAMS
test_read_and_translate: test_read_and_translate.c kbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)
test_findkbd: test_findkbd.c kbd.o
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h, $^)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<
