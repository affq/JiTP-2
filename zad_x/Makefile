CXX = gcc
CXXFLAGS = -Wall -Wpedantic -Wextra -Og -g
SRCS = zad_x.c $(shell find -name "*.c")
OUT = example.out
LIBS = -lfltk -lfltk_images -lX11 -lXext

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

run: all
	./$(OUT)

clean:
	rm -f $(OUT)