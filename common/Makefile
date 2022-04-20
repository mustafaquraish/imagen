# nice lil make file

LIB :=  -lpng
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIB += -lX11 -lGL -lpthread -lstdc++fs
endif
ifeq ($(UNAME_S),Darwin)
	LIB += -framework OpenGL -framework GLUT
endif
CCFLAGS += -std=c++17 -g -I ../common/

BUILD = build
HTML = $(BUILD)/index.html

EMFLAGS += -s ALLOW_MEMORY_GROWTH=1
EMFLAGS += -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2
EMFLAGS += -s USE_LIBPNG=1 -O3

all:
	g++ $(CCFLAGS) $(SRC) -o $(EXE) $(LIB) 

debug:
	g++ $(LIB) $(CCFLAGS) $(SRC) -o $(EXE)

$(BUILD):
	mkdir -p $@

ems: $(BUILD)
	em++ $(CCFLAGS) $(EMFLAGS) $(SRC) -o $(HTML)

clean:
	rm -rf $(EXE) $(BUILD)
