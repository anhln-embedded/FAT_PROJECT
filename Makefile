TARGET = output/program_run.exe

CC = gcc

CFLAGS = -Wall -Werror

SRC = $(wildcard *.c)

OBJ = $(SRC:%.c=obj/%.o)

$(TARGET): $(OBJ)
	if not exist output mkdir output
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	$(TARGET) floppy.img

obj/%.o: %.c
	if not exist obj mkdir obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q obj\*.o
	if exist output rmdir /S /Q output
	if exist obj rmdir /S /Q obj

run: $(TARGET)
	$(TARGET) floppy.img
