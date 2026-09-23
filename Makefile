NAME = libclog.a

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=gnu11
UFLAGS = -DCLOG_ANSI_COLOR

AR = ar
ARFLAGS = rcs

BIN_DIR = bin
OBJ_DIR = obj

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/$(NAME)

.PHONY: all clean fclean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ_DIR)/%.o: %.c $(wildcard *.h)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(UFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)

fclean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
