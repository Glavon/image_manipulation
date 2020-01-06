#ifndef png_file
#define png_file
#include <png.h>
#include "image.h"
class PNG : public Image {
    public:
    /*
    Copy png constructor, duplicates png
    */
    PNG(const PNG &png);
    /*
    Constructs PNG from reading in filename
    */
    PNG(const std::string &filename);
    /*
    Constructs PNG from given width and height, initalizes
    as blank black image
    */
    PNG(int32_t width, int32_t height);
    /*
    Destructs PNG, nothing special
    */
    ~PNG();
    /*
    
    */
    void update_metadata();
    void print(std::ostream &os) const;
    void read(const std::string &filename);
    void write() const;
    friend std::ostream& operator<<(std::ostream& os, const PNG &png) {
        png.print(os);
        return os;
    }
    private:
    png_structp png;
    png_infop pngi;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;
};
#endif