# Compiler setup
CC = gcc
CFLAGS = -Wall -Wextra --std=c99

# Define targets
TARGET = image_editor

# Source files
SRCS = image_editor.c auxiliary.c main_functions.c

# Object files
OBJS = $(SRCS:.c=.o)

# Build rule
all: $(TARGET)

# Target for building the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lm -o $(TARGET)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)
