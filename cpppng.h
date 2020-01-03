#ifndef png_file
#define png_file
#include <png.h>
#include "image.h"
class PNG : public Image {
    public:
    PNG(const PNG &bmp);
    PNG(std::string filename);
    PNG(int32_t width, int32_t height);
    ~PNG();
    void update_header();
    void print(std::ostream &os) const;
    void read(const std::string &filename);
    void write(const std::string &filename) const;
    friend std::ostream& operator<<(std::ostream& os, const PNG &png) {
        png.print(os);
        return os;
    }
    private:
    png_struct *pngs;
    png_info *pngi;
};
#endif