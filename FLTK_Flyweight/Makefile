all: flyweight.cpp non_flyweight.cpp
	g++ -std=c++17 -o flyweight flyweight.cpp `fltk-config --use-images --cxxflags --ldflags`
	g++ -std=c++17 -o nonflyweight non_flyweight.cpp `fltk-config --use-images --cxxflags --ldflags`

run:
	./flyweight &
	./nonflyweight &

clean:
	rm flyweight nonflyweight