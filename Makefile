CC = g++
CFLAGS = -I"C:\Users\devan\Documents\Libraries\SFML-2.6.1\include" -I"include" -DSFML_STATIC
LDFLAGS = -L"C:\Users\devan\Documents\Libraries\SFML-2.6.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -static

# Source files
SOURCES = src/main.cpp src/game.cpp src/ball.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)
