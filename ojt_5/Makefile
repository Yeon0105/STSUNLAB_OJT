#Makefile

#Compiler
CC = gcc

#Source file
MAIN=ojt_5.c

#Target executable
TARGET = ojt_5
TARGETS_CLEAN = $(TARGET) x_*.bin

#Default target
all: $(TARGET)

$(TARGET): $(MAIN)
	$(CC) -o $(TARGET) $(MAIN)

clean:
	rm -f $(TARGET) $(TARGETS_CLEAN)