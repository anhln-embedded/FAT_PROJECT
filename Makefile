TARGET = output/program_run.exe

CC = gcc

CFLAGS = -Wall -Werror

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	if not exist output mkdir output
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	$(TARGET) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o
	rmdir /S /Q output

run: $(TARGET)
	$(TARGET)
