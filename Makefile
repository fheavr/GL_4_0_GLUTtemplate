
CC = g++
LFLAGS = -lGL -lfreeglut -lGLEW
CFLAGS = -g $(LFLAGS)

BIN = a1_shaderIntro
CLS = main ControlState IBuffer Mesh ShaderUtils VBuffer WorldState
SRC = $(CLS:%=%.cpp)
OBJ = $(SRC:%.cpp=build/%.o)

default: build
build: $(BIN) 

$(BIN): build/main.o $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^

build/%.o: %.cpp
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *~ $(OBJ) $(BIN) build
