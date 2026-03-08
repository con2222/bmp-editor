CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = bmp-editor
SRCS = main.c bmp_io.c bmp_filters.c cli.c
OBJS = $(SRCS:.c=.o)
DEPS = bmp.h

ifeq ($(OS),Windows_NT)
    RM = del /q /f
    TARGET_EXE = $(TARGET).exe
else
    RM = rm -f
    TARGET_EXE = $(TARGET)
endif

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET_EXE)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET_EXE)

.PHONY: all clean