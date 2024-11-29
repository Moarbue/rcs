CC      := clang
CFLAGS  := -Wall -Wextra -O0 -ggdb3 -c -I./extern/include -I./include
LDFLAGS := -L./extern/lib -lopengl32 -lgdi32 -lglfw3 -lglad

BIN_DIR    := bin
SRC_DIR    := src
SHADER_DIR := shaders

BIN := $(BIN_DIR)/rcs
OBJ := $(BIN_DIR)/main.o 	\
	   $(BIN_DIR)/smath.o	\
	   $(BIN_DIR)/vec.o

.PHONY: all mkbindir cpshaders clean

all: mkbindir cpshaders $(BIN)

mkbindir:
	mkdir -p $(BIN_DIR)

cpshaders:
	cp -r $(SHADER_DIR) $(BIN_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(BIN):	$(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(BIN) $(BIN)/$(SHADER_DIR)