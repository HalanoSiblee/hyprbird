all:
	$(CXX) -shared -fPIC --no-gnu-unique main.cpp -o hyprbird.so -g `pkg-config --cflags pixman-1 libdrm hyprland` -lSDL2 -std=c++2b -O2 -s
clean:
	rm ./hyprbird.so
