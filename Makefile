TARGET	= libftpp.a
CC		= g++
BUILD	?= debug # defualt to debug

STD_FLAGS	= -std=c++23 -Wall -Werror -Wextra -Iinclude
DEBUG_FLAGS	= $(STD_FLAGS) -g3 -O0
RELEASE_FLAGS = $(STD_FLAGS) -03 -march=native -flto
CFLAGS = $(if $(filter debug,$(BUILD)),$(DEBUG_FLAGS),$(RELEASE_FLAGS))

SRC_DIR	= src
OBJ_DIR	= obj

SRCS	= $(wildcard $(SRC_DIR)/*.cpp)
OBJS	= $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)


$(TARGET) : $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	@echo "Cleaning obj files..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Removing library..."
	rm $(TARGET)

re: fclean
	make all

.PHONY: all clean fclean re
