CC=g++
CFLAGS=-I. -lm -lasound -lSDL2 -lpthread
CFLAG=-I.
BUILDDIR=build
SRCDIR=src
BINDIR=bin
TESTDIR=tests
OBJDIR=$(BINDIR)/obj

EXE=accelerando

SOURCES = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: build

build: $(OBJ)
	$(CC) -o $(EXE) $^ $(CFLAGS)
	cp $(EXE) $(BINDIR)/$(EXE)

debug: CFLAGS += -DDEBUG -g
debug: CFLAG += -DDEBUG -g
debug: build

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAG)

test_midi_reader: $(TESTDIR)/test_midi_reader.c $(OBJDIR)/midi_reader.o
	mkdir -p $(BINDIR)/$(TESTDIR)
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	rm $@

test_keyboard_UserEvent: $(TESTDIR)/test_keyboard_UserEvent.c $(OBJDIR)/keyboard_poll.o
	mkdir -p $(BINDIR)/$(TESTDIR)
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	rm $@

test_gpio: test/test_gpio_map.c $(OBJDIR)/gpio.o
	mkdir -p $(BINDIR)/$(TESTDIR)
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	rm $@

clean:
	rm -rf $(OBJDIR) $(EXE) $(BINDIR)

run:
	./$(EXE)
