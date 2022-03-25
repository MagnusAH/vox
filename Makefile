# remove .exe if on linux or compiling for linux
CC 			= clang.exe
EXECUTABLE 	= vox.exe

SRCDIR 		= src
INCDIR		= include
OBJDIR 		=
BINDIR		=
SUBDIRS		= alloc client common core gl gui hash math server sys time

# change this to working drive letter if using wsl (must be lowercase)
WSL_DRIVE_LETTER = c

PPFLAGS = -D_CRT_SECURE_NO_DEPRECATE
CFLAGS = -msse4.2 -mavx2 -mfma -std=c11
LFLAGS =

# nono touch

ifneq ($(patsubst %.exe, %, $(CC)), $(CC))
	WINDOWS = true
endif

ifdef WINDOWS
	CWD:=$(patsubst /mnt/$(WSL_DRIVE_LETTER)/%, $(WSL_DRIVE_LETTER):/%, $(shell pwd))
	PPFLAGS +=
	LFLAGS += -L$(CWD)/lib/win -lopengl32 -lglfw3dll
	BINDIR = dist/win
else
	CWD:=$(shell pwd)
	PPFLAGS += -D_POSIX_C_SOURCE=199309L
	LFLAGS += -L$(CWD)/lib/linux -lpthread -lrt -ldl -lm -l:libglfw.so.3 -Wl,-rpath,.
	BINDIR = dist/linux
endif


OBJDIR = $(BINDIR)/build
VPATH = $(CWD)

CFLAGS += -I$(CWD)/$(SRCDIR) -I$(CWD)/$(INCDIR)

SRC_C := $(patsubst $(SRCDIR)/%.c, %.c, $(wildcard $(SRCDIR)/*.c))
SRC_C += $(foreach dir, $(SUBDIRS), $(patsubst $(SRCDIR)/%.c, %.c, $(wildcard $(SRCDIR)/$(dir)/*.c)))

OBJ_C := $(patsubst %.c, $(OBJDIR)/%.o, $(SRC_C))

.PHONY: raw build release compile clean run clear_screen dist package

raw: CFLAGS +=
raw: PPFLAGS += -DDEBUG
raw: compile

build: CFLAGS += -O2
build: PPFLAGS += -DDEBUG
build: compile

release: CFLAGS += -O3 -Werror
release: PPFLAGS += -DRELEASE
release: clean compile

compile: clear_screen $(OBJ_C)
	$(CC) $(OBJ_C) -o $(BINDIR)/$(EXECUTABLE) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(PPFLAGS) -c $^ -o $@

clean:
	@rm -rf $(OBJDIR)/*

run: clear_screen
	@rm -f $(BINDIR)/logs/*.log
	@cd $(BINDIR) && ./$(EXECUTABLE) $(ARGS)

dist:
	@mkdir -p dist/linux dist/win
	@cp -u lib/linux/* dist/linux

package:
	@mkdir -p package
	@rsync -av --progress $(BINDIR) package --exclude build --exclude logs

clear_screen:
	@echo "!========< NEW MAKE TASK >========!"