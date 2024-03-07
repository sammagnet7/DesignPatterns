linux: flyweight.cpp non_flyweight.cpp
	g++ -std=c++17 -o flyweight flyweight.cpp `fltk-config --use-images --cxxflags --ldflags`
	g++ -std=c++17 -o nonflyweight non_flyweight.cpp `fltk-config --use-images --cxxflags --ldflags`

mac: flyweight.cpp non_flyweight.cpp
	g++ -std=c++17 -o flyweight flyweight.cpp `fltk-config --cxxflags --ldflags` -lfltk_images
	g++ -std=c++17 -o nonflyweight non_flyweight.cpp `fltk-config --cxxflags --ldflags` -lfltk_images

run:
	./flyweight &
	./nonflyweight &

clean:
	rm flyweight nonflyweight