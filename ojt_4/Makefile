#Makefile

#Compiler
CC = gcc

#Source file
MAIN=ojt_4.c

#Target executable
TARGET_1 = ojt_4
TARGET_2 = cal_result.txt

#Default target
all: $(TARGET_1)

$(TARGET_1): $(MAIN)
	$(CC) -o $(TARGET_1) $(MAIN)

clean:
	rm -f $(TARGET_1) $(TARGET_2)