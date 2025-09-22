CC		:= gcc
CFLAGS	:= -Wall -Wextra -O0 -ggdb3 -c -I./extern/include -I./include
LDFLAGS :=

ifeq ($(OS), Windows_NT)
	LDFLAGS += -L./extern/lib -lgdi32 -lglfw3 -lfreetype
else
	LDFLAGS += -lm -lglfw
endif

BIN_DIR		:= bin
SRC_DIR		:= src
OBJ_DIR		:= $(BIN_DIR)/obj
SHADER_DIR	:= shaders
FONT_DIR    := fonts

BIN		:= $(BIN_DIR)/rcs
OBJ		:= $(OBJ_DIR)/main.o 		\
	   	   $(OBJ_DIR)/smath.o		\
	   	   $(OBJ_DIR)/vec.o			\
	   	   $(OBJ_DIR)/mat.o			\
	   	   $(OBJ_DIR)/quat.o		\
	   	   $(OBJ_DIR)/color.o		\
	   	   $(OBJ_DIR)/logging.o		\
	   	   $(OBJ_DIR)/window.o		\
	   	   $(OBJ_DIR)/util.o		\
	   	   $(OBJ_DIR)/shader.o		\
	   	   $(OBJ_DIR)/vertex.o		\
	   	   $(OBJ_DIR)/cubie.o		\
	   	   $(OBJ_DIR)/cube.o		\
	   	   $(OBJ_DIR)/animation.o	\
	   	   $(OBJ_DIR)/camera.o		\
	   	   $(OBJ_DIR)/config.o		\
	   	   $(OBJ_DIR)/font.o
SHADERS := $(BIN_DIR)/$(SHADER_DIR)/cube.vert	\
		   $(BIN_DIR)/$(SHADER_DIR)/cube.frag	\
		   $(BIN_DIR)/$(SHADER_DIR)/font.vert	\
		   $(BIN_DIR)/$(SHADER_DIR)/font.frag
FONTS   := $(BIN_DIR)/$(FONT_DIR)/arial.ttf				\
		   $(BIN_DIR)/$(FONT_DIR)/Minecrafter.Alt.ttf


# Append gl.o to objects
OBJ += $(OBJ_DIR)/gl.o

.PHONY: all clean

all: $(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/$(SHADER_DIR) $(BIN_DIR)/$(FONT_DIR) $(BIN) $(SHADERS) $(FONTS)

# make directories

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(SHADER_DIR):
	mkdir -p $(BIN_DIR)/$(SHADER_DIR)

$(BIN_DIR)/$(FONT_DIR):
	mkdir -p $(BIN_DIR)/$(FONT_DIR)


# copy shaders
$(BIN_DIR)/$(SHADER_DIR)/%: $(SHADER_DIR)/%
	cp $< $@

# copy fonts
$(BIN_DIR)/$(FONT_DIR)/%: $(FONT_DIR)/%
	cp $< $@


# Compile all .c files inside $(SRC_DIR) to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile gl.c to gl.o
$(OBJ_DIR)/gl.o: extern/lib/gl.c
	$(CC) $(CFLAGS) -o $@ $<

# Compile .o files to executable and link with libs
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Remove all .o files and executables
clean:
	rm -rf $(OBJ) $(BIN) $(SHADERS)