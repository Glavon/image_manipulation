#include "cpppng.h"
#include <string>
using namespace std;

PNG::PNG(const PNG &png) {

}
PNG::PNG(const string &filename) {
    read(filename);
}
PNG::PNG(int32_t width, int32_t height) {

}
PNG::~PNG() {

}
void PNG::update_metadata() {

}
void PNG::print(ostream &os) const {
    os << "Width x Height" << endl;
    os << png_get_image_width(png,pngi) << "x" << png_get_image_height(png,pngi) << endl;
}
void PNG::read(const string &filename) {
    FILE *inp = fopen(filename.c_str(), "rb");
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    pngi = png_create_info_struct(png);
    png_init_io(png, inp);
    
    png_read_info(png, pngi);
    width = png_get_image_width(png, pngi);
    height = png_get_image_height(png, pngi);
    color_type = png_get_color_type(png, pngi);
    bit_depth = png_get_bit_depth(png, pngi);

    if(bit_depth == 16) {
        png_set_strip_16(png);
    }
    if(color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }
    if(png_get_valid(png, pngi, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }
    if(color_type == PNG_COLOR_TYPE_RGB ||
    color_type == PNG_COLOR_TYPE_GRAY ||
    color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }
    if(color_type == PNG_COLOR_TYPE_GRAY || 
    color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }
    png_read_update_info(png, pngi);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int i = 0; i < height; ++i) {
        row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png, pngi));
    }
    png_read_image(png, row_pointers);
    for(int r = 0; r < height; ++r) {

        for(int c = 0; c < width; ++c) {
            png_bytep og_pix = &(row_pointers[r][c * 4]);
            Pixel *p = new Pixel(og_pix[0], og_pix[1], og_pix[2]);
            pixels->push_back(p);
        }
    }
    fclose(inp);
}
void PNG::write(const string &filename) const {

}