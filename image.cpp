#include "image.h"
#include <iostream>
using namespace std;

Image::Image() {
    pixels = new vector<Pixel*>;
}
Image::~Image() {
    clear_pixels();
    delete pixels;
}
void Image::set_image_name(string name) {
    image_name = name;
}
void Image::compress(int factor) {
    for(int i = 0; i < factor; ++i) {
        compress();
    }
}
void Image::rotate(string direction) {
    if(direction == "right") {
        rotate_right();
    } else if(direction == "left") {
        rotate_left();
    } else {
        return;
    }
    return;
}
void Image::flip(string axis) {
    if(axis == "vertical") {
        flip_vertical();
    } else if(axis == "horizontal") {
        flip_horizontal();
    }
}
void Image::enhance(int factor) {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    int new_width = width*factor;
    int new_height = height*factor;
    for(int r = 0; r < height; ++r) {
        for(int i = 0; i < factor; ++i) {
            for(int c = 0; c < width; ++c) {
                for(int j = 0; j < factor; ++j) {
                    Pixel *p = new Pixel(*pixels->at(r*width+c));
                    new_data->push_back(p);
                }
            }
        }
    }
    clear_pixels();
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    width = new_width;
    height = new_height;
    update_metadata();
    return;
}
void Image::invert() {
    for(auto p: *pixels) {
        p->invert();
    }
}
void Image::remove_color(string color) {
    for(auto p: *pixels) {
        p->remove_color(color);
    }
}
void Image::contrast(int num) {
    for(auto p: *pixels) {
        p->contrast(num);
    }
}
void Image::flip_horizontal() {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    for(int r = 0; r < height; ++r) {
        for(int c = width-1; c >= 0; --c) {
            new_data->push_back(pixels->at(r*width + c));
        }
    }
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    update_metadata();
    return;
}
void Image::flip_vertical() {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    for(int r = height-1; r >= 0; --r) {
        for(int c = 0; c < width; ++c) {
            new_data->push_back(pixels->at(r*width + c));
        }
    }
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    update_metadata();
    return;
}
void Image::rotate_right() {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    for(int c = width-1; c >= 0; --c) {
        for(int r = 0; r < height; ++r) {
            new_data->push_back(pixels->at(r*width + c));
        }
    }
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    uint32_t temp = width;
    width = height;
    height = temp;
    update_metadata();
    return;
}
void Image::rotate_left() {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    for(int c = 0; c < width; ++c) {
        for(int r = height-1; r >= 0; --r) {
            new_data->push_back(pixels->at(r*width + c));
        }
    }
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    uint32_t temp = width;
    width = height;
    height = temp;
    update_metadata();
    return;
}
void Image::compress() {
    vector<Pixel*> *new_data = new vector<Pixel*>;
    int multiple = least_common_multiple(width, height);
    int new_width = width/multiple;
    int new_height = height/multiple;
    for(int pg_r = 0; pg_r < new_height; ++pg_r) {
        for(int pg_c = 0; pg_c < new_width; ++pg_c) {
            int red{0}, blue{0}, green{0};
            for(int r = 0; r < multiple; ++r) {
                for(int c = 0; c < multiple; ++c) {
                    vector<int> rgb;
                    rgb = pixels->at(r*width + c + pg_r*width*multiple + pg_c*multiple)->get_rgb();
                    red += rgb.at(0);
                    green += rgb.at(1);
                    blue += rgb.at(2);
                }
            }
            red = red/(multiple*multiple);
            green = green/(multiple*multiple);
            blue = blue/(multiple*multiple);
            Pixel *p = new Pixel(red, green, blue);
            new_data->push_back(p);
        }
    }
    clear_pixels();
    vector<Pixel*> *old_data = pixels;
    pixels = new_data;
    delete old_data;
    width = new_width;
    height = new_height;
    update_metadata();
    return;
}
string Image::hex(int i) const {
    string output = "";
    int remaind;
    double result;
    while(i != 0) {
        result = double(i)/16;
        remaind = (result - (i/16)) * 16;
        switch(remaind) {
            case 10:
            output = "A" + output;
            break;
            case 11:
            output = "B" + output;
            break;
            case 12:
            output = "C" + output;
            break;
            case 13:
            output = "D" + output;
            break;
            case 14:
            output = "E" + output;
            break;
            case 15:
            output = "F" + output;
            break;
            default:
            output = to_string(remaind) + output;
            break;
        }
        i = result;
    }
    return output;
    
}
string Image::convert_bytes(double bytes) const  {
    string units = "";
    if(bytes > 1024*1024*1024) {
        bytes = bytes/(1024*1024*1024);
        units = "GB";
    } else if (bytes > 1024*1024) {
        bytes = bytes/(1024*1024);
        units = "MB";
    } else if (bytes > 1024) {
        bytes = bytes/(1024);
        units = "KB";
    } else {
        units = "B";
    }
    return to_string(bytes) + " " + units;
}
int Image::least_common_multiple(int num1, int num2) const {
    int factor = 2;
    while(num1 % factor != 0 && num2 % factor != 0) {
        ++factor;
    }
    return factor;
}
void Image::clear_pixels() {
    for(int i = 0; pixels->size() > 0; ++i) {
        delete pixels->at(pixels->size()-1);
        pixels->pop_back();
    }
}
Image::Pixel::Pixel() {}
Image::Pixel::Pixel(const Pixel &p) {
    r = p.r;
    g = p.g;
    b = p.b;
    
}
Image::Pixel::Pixel(int red, int green, int blue) {
    r = uint8_t(red);
    g = uint8_t(green);
    b = uint8_t(blue);
}
void Image::Pixel::invert() {
    r = 255-r;
    g = 255-g;
    b = 255-b;
    return;
}
void Image::Pixel::remove_color(string color) {
    if(color == "red") {
        r = 0;
    } else if (color == "blue") {
        b = 0;
    } else if (color == "green") {
        g = 0;
    }
}
vector<int> Image::Pixel::get_rgb() const {
    return {r,g,b};
    
}
void Image::Pixel::set_rgb(int red, int green, int blue) {
    r = uint8_t(red);
    g = uint8_t(green);
    b = uint8_t(blue);
    return;
}
void Image::Pixel::contrast(int num) {
    //TODO fix negative contrast
    if(int(r) + num > 255) {
        r = 255;
    } else if(int(r) - num < 0) {
        r = 0;
    } else {
        if(r > 127) {
            r = r + num;
        } else {
            r = r - num;
        }
    }
    if(int(g) + num > 255) {
        g = 255;
    } else if(int(g) - num < 0) {
        g = 0;
    } else {
        if(g > 127) {
            g = g + num;
        } else {
            g = g - num;
        }
    }
    if(int(b) + num > 255) {
        b = 255;
    } else if(int(b) - num < 0) {
        b = 0;
    } else {
        if(b > 127) {
            b = b + num;
        } else {
            b = b - num;
        }
    }
}