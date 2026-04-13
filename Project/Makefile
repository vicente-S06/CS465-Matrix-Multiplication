CC   = gcc
OPTS = -Wall -O0

TARGET = bin/p1
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: clean $(TARGET)	

clean:
	@rm -f obj/*.o
	@rm -f bin/*
	@echo obj and bin directories have been cleared

$(TARGET): $(OBJ)
	$(CC) -o $@ $?

obj/%.o : src/%.c
	$(CC) -c $(OPTS) $< -o $@ -Iinc
