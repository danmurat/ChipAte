# CC = g++
# CFLAGS = -std=c++20 -ggdb -pedantic-errors -Wall -Weffc++ -Werr -Wextra -Wsign-conversion -Werror
# INCLUDES = -I /opt/homebrew/Cellar/sdl2/2.26.2/include
# LINKS = -L /opt/homebrew/Cellar/sdl2/2.26.2/lib
# LINK_FLAGS = -lSDL2
# APP = chip8
# SRC_FILES = main.cpp cpu.cpp screen.cpp dssemble.cpp
# OBJ_FILES = chip8.o
# .c.o: $(SRC_FILES)
# 	 $(CC) $(CFLAGS) ${INCLUDES} -c $^
# all:
# 	 $(CC) $(CFLAGS) -o $(APP) ${LINKS} $< $(LINK_FLAGS)
# clean:
# 	rm -rf $(OBJ_FILES) $(APP)

make chip8:
	g++ -std=c++20 -ggdb -pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion -Werror -I /opt/homebrew/Cellar/sdl2/2.26.2/include -L /opt/homebrew/Cellar/sdl2/2.26.2/lib -l SDL2-2.0.0 -o chip8 main.cpp cpu.cpp graphics.cpp
