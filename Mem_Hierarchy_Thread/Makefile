CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=gnu99 -O0 -fopenmp
LIBS = -lm 

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./inc
BINDIR = ./bin

TARGET = $(BINDIR)/Mem_Hierarchy
DEBUG_TARGET = $(BINDIR)/DEBUG_Mem_Hierarchy
INCLUDE = $(addprefix -I,$(INCDIR))

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))


all: $(TARGET)	

debug: CFLAGS += -g
debug: $(DEBUG_TARGET)

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(BINDIR)/*
	@echo obj and bin directories have been cleared

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(DEBUG_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@ -I$(INCDIR) $(LIBS)
