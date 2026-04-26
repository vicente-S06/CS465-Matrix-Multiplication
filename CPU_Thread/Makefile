CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=gnu99 -O0 -fopenmp
LIBS = -lm 

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./inc
BINDIR = ./bin

TARGET1 = $(BINDIR)/Mem_Hierarchy
DEBUG_TARGET1 = $(BINDIR)/DEBUG_Mem_Hierarchy
TARGET2 = $(BINDIR)/OpenMP_Thread

T1OBJS=$(addprefix $(OBJDIR)/,memH_main.o matrix.o util.o trial.o)
T2OBJS=$(addprefix $(OBJDIR)/,omp_main.o matrix.o util.o trial.o)

all: $(TARGET1)	$(TARGET2)

test:
	@echo $(T1OBJS)
	@echo $(T2OBJS)

debug: CFLAGS += -g
debug: $(DEBUG_TARGET1)

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(BINDIR)/*
	@echo obj and bin directories have been cleared

$(TARGET1): $(T1OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(TARGET2): $(T2OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(DEBUG_TARGET1): $(T1OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@ -I$(INCDIR) $(LIBS)
