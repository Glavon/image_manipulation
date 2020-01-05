build: image.cpp png.cpp bmp.cpp edit.cpp
	g++ image.cpp png.cpp bmp.cpp edit.cpp -lpng -I/usr/include/libpng16/ -o edit.exe