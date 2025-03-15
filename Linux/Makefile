## 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -g

# 最终生成的可执行文件名称
TARGET = huffman

# 默认目标
all: $(TARGET)

# 链接阶段：将目标文件生成可执行文件
$(TARGET): main.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o

# 编译阶段：生成 main.o
main.o: main.c huffman_linux.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# 清理生成文件
clean:
	rm -f $(TARGET) main.o

# 声明伪目标
.PHONY: all clean
