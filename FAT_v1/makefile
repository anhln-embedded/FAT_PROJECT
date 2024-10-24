CC = gcc

TARGET = protemp

LIB = DirectoryEntry.h dirNode.h fat_lib.h read_infor.h readClusterData.h getNextCluster.h

SOURCE = DirectoryEntry.c dirNode.c fat_lib.c read_infor.c readClusterData.c main.c getNextCluster.c

all: $(TARGET)
$(TARGET): $(SOURCE) $(LIB)
	$(CC) -o $(TARGET) $(SOURCE)
	