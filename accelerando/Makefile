CC=g++
CFLAGS=-I. -lm -lasound -lSDL2 -lSDL2_image -lSDL2_ttf -lpthread
CFLAG=-I.
BUILDDIR=build
SRCDIR=src
BINDIR=bin
TESTDIR=tests
OBJDIR=$(BINDIR)/obj

EXE=accelerando

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

all: build

build: $(OBJ)
	$(CC) -o $(EXE) $^ $(CFLAGS)
	cp $(EXE) $(BINDIR)/$(EXE)

debug: CFLAGS += -DDEBUG -g
debug: CFLAG += -DDEBUG -g
debug: build

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAG)


test_%: $(TESTDIR)/test_%.cpp $(filter-out $(OBJDIR)/main.o, $(OBJ))
	mkdir -p $(BINDIR)/$(TESTDIR)
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	rm $@

#test_keyboard_UserEvent: $(TESTDIR)/test_keyboard_UserEvent.cpp $(OBJDIR)/keyboard_poll.o
#	mkdir -p $(BINDIR)/$(TESTDIR)
#	$(CC) -o $@ $^ $(CFLAGS)
#	./$@
#	rm $@
#
#test_gpio: $(TESTDIR)/test_gpio_map.cpp $(OBJDIR)/gpio.o
#	mkdir -p $(BINDIR)/$(TESTDIR)
#	$(CC) -o $@ $^ $(CFLAGS)
#	./$@
#	rm $@

clean:
	rm -rf $(OBJDIR) $(EXE) $(BINDIR)

run:
	./$(EXE)
