C = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: main

main: main.o model.o walls.o tools.o
	$(C) build/main.o build/model.o build/walls.o build/tools.o -o main $(OPT) $(CFLAGS) $(SFMLFLAGS)

%.o: %.cpp
	$(C) -c $< -o build/$@ $(OPT) $(CFLAGS) $(SFMLFLAGS) $(CFLAGS)

clean:
	rm -f build/*.o main

run:
	./main

help:
	@echo "Make command runs g++ compiler"
	@echo "Requires SFML (dynamic linking)"
	@echo "Use make (all) / make clean / make run"
	@echo "Use OPT=-O2 (or big graphs will lag horribly)"

.PHONY: all clean run help