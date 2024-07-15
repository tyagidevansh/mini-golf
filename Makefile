# Compiler and flags
CC = g++
CFLAGS = -I"C:\Users\devan\Documents\Libraries\SFML-2.6.1\include" -I"include"
LDFLAGS = -L"C:\Users\devan\Documents\Libraries\SFML-2.6.1\lib" \
          -lfreetype -lopengl32 -lwinmm -lgdi32 \
          -mwindows

# SFML dynamic libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Source files
SOURCES = src/main.cpp src/game.cpp src/ball.cpp src/map.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(SFML_LIBS)

src/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
