#include <string>
#include <iostream>
#include "bmp.h"
using namespace std;

int main() {
    BMP bmp = BMP("corgi.bmp");
    cout << "enhancing" << endl;
    bmp.enhance(2);
    cout << "compressing" << endl;
    bmp.compress(1);
    cout << "rotating right" << endl;
    bmp.rotate("right");
    cout << "rotating right" << endl;
    bmp.rotate("right");
    cout << "flipping vertical" << endl;
    bmp.flip("vertical");
    cout << "flipping horizontal" << endl;
    bmp.flip("horizontal");
    cout << "removing red" << endl;
    bmp.remove_color("red");
    cout << "inverting" << endl;
    bmp.invert();
    cout << "inverting" << endl;
    bmp.invert();
    cout << "inverting" << endl;
    bmp.invert();
    cout << "compressing" << endl;
    bmp.compress(1);
    cout << "writing" << endl;
    bmp.write("test.bmp");
    return 0;
}