CC = nvcc
CFLAGS =  -O0 
LIBS = -lm 

SRCDIR = ./src
OBJDIR = ./obj
INCDIR = ./inc
BINDIR = ./bin

TARGET = $(BINDIR)/GPU_Thread
DEBUG_TARGET = $(BINDIR)/DEBUG_GPU_Thread
INCLUDE = $(addprefix -I,$(INCDIR))

C_SRCS = $(wildcard $(SRCDIR)/*.c)
C_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(C_SRCS))
CPP_SRCS = $(wildcard $(SRCDIR)/*.cpp)
CPP_OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.opp, $(CPP_SRCS))

OBJS = $(C_OBJS) $(CPP_OBJS)

default: $(TARGET)	

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

$(OBJDIR)/%.opp : $(SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@ -I$(INCDIR) $(LIBS)

