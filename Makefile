TARGET = output/program_run.exe

CC = gcc

CFLAGS = -Wall -Werror

SRC = $(wildcard *.c)

OBJ = $(SRC:%.c=obj/%.o)

# Tạo thư mục output nếu chưa tồn tại
$(TARGET): $(OBJ)
	@if [ ! -d output ]; then mkdir output; fi
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	$(TARGET) floppy.img

# Tạo thư mục obj nếu chưa tồn tại
obj/%.o: %.c
	@if [ ! -d obj ]; then mkdir obj; fi
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@del /Q obj\*.o
	@if exist output rmdir /S /Q output
	@if exist obj rmdir /S /Q obj

run: $(TARGET)
	$(TARGET) floppy.img
