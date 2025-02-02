CC = gcc
CFLAGS = -Wall -g
TARGET = freq
OBJS = main.o freq.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

freq.o: freq.c
	$(CC) $(CFLAGS) -c freq.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)