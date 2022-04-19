# nice lil make file

SRC = main.cpp

LIB = -framework OpenGL -framework GLUT -lpng
CCFLAGS += -std=c++17 -g

EXE = interference

BUILD = build
HTML = $(BUILD)/index.html


EMFLAGS += -s ALLOW_MEMORY_GROWTH=1
EMFLAGS += -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2
EMFLAGS += -s USE_LIBPNG=1 -O3

all:
	g++ $(CCFLAGS) $(LIB) $(SRC) -o $(EXE) -O3

debug:
	g++ $(CCFLAGS) $(LIB) $(SRC) -o $(EXE)

$(BUILD):
	mkdir -p $@

ems: $(BUILD)
	em++ $(SRC) -o $(HTML) $(CCFLAGS) $(EMFLAGS)

clean:
	rm -rf $(EXE) $(BUILD)