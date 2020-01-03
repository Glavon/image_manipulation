#include <string>
#include <iostream>
#include "bmp.h"
#include "cpppng.h"
using namespace std;

int main() {
    PNG png = PNG("corgi.png");
    cout << png << endl;
}