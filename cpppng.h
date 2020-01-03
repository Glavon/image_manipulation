#ifndef png_file
#define png_file
#include <png.h>
#include "image.h"
class PNG : public Image {
    public:
    PNG(const PNG &png);
    PNG(const std::string &filename);
    PNG(int32_t width, int32_t height);
    ~PNG();
    void update_metadata();
    void print(std::ostream &os) const;
    void read(const std::string &filename);
    void write(const std::string &filename) const;
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