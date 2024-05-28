#Makefile 

#Compiler
CC = gcc 

#Source file
MAIN = hello.c

#Target executable 
TARGET = hello

#Object file
OBJS = $(MAIN:.c=.o)

#Default target
all: $(TARGET)

#Rule to build object files
%.o: %.c
	$(CC) -c -o $@ $<

#Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)

