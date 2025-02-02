# Makefile for cube_avg project

CC = gcc
CFLAGS = -Wall -g
TARGET = cube_avg
OBJS = main.o cube_avg.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

cube_avg.o: cube_avg.c
	$(CC) $(CFLAGS) -c cube_avg.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)