# ---- tiny shell Makefile ----

CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c23 -g
TARGET  := tsh

SRC     := tsh.c
OBJ     := $(SRC:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

