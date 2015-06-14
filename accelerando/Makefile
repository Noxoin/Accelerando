CC=gcc
CFLAGS=-I. -lm
CFLAG=-I.
BUILDDIR=build
SRCDIR=src
BINDIR=bin
OBJDIR=$(BINDIR)/obj

EXE=accelerando

SOURCES = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(SRCDIR)/*.h)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: build

build: $(OBJ)
	$(CC) -o $(EXE) $^ $(CFLAGS)
	mkdir -p $(BINDIR)
	cp $(EXE) $(BINDIR)/$(EXE)

debug: CFLAGS += -DDEBUG -g
debug: CFLAG += -DDEBUG -g
debug: build

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAG)

clean:
	rm -rf $(OBJDIR) $(EXE) $(BINDIR)

#run:
#	@echo "============================"
#	@echo "Starting Accelerando        "
#	@echo "============================"
#	./$(EXE)
