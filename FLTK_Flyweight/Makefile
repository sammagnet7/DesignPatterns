all:
	g++ -std=c++17 -o flyweight flyweight.cpp `fltk-config --cxxflags --ldflags` -lfltk_images
	g++ -std=c++17 -o nonflyweight non_flyweight.cpp `fltk-config --cxxflags --ldflags` -lfltk_images

run:
	./flyweight &
	./nonflyweight &

clean:
	rm flyweight nonflyweight