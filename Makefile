#OBJS specifies source files
OBJS = diffusion-engine.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS
COMPILER_FLAGS = -pedantic -Wall -Werror -std=c++11

#LINKER_FLAGS
LINKER_FLAGS = -lglew -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

#OBJ_NAME
OBJ_NAME = diffusion-engine

OBJS_TEST = diffusion-engine-test.cpp
OBJ_NAME_TEST = diffusion-engine-test

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

.PHONY: test
test :
	$(CC) $(OBJS_TEST) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME_TEST) && ./$(OBJ_NAME_TEST)
