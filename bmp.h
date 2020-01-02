#ifndef bmp_file
#define bmp_file
#include <string>
#include "image.h"
class BMP : public Image {
    public:
    BMP(std::string filename);
    BMP(int32_t width, int32_t height);
    ~BMP();
    void update_header();
    void print(std::ostream &os) const;
    void read(const std::string &filename);
    void write(const std::string &filename);
    friend std::ostream& operator<<(std::ostream& os, const BMP &bmp) {
        bmp.print(os);
        return os;
    }
    private:
    uint32_t row_stride{0};
    uint32_t make_stride_aligned(uint32_t align_stride);
    #pragma pack(push, 1)
    struct FileHeader {
        uint16_t file_type{0x4D42};
        uint32_t file_size{0};
        uint16_t reserved1{0};
        uint16_t reserved2{0};
        uint32_t offset_data{0};
    };
    #pragma pack(pop)
    struct InfoHeader {
        uint32_t size{0};
        int32_t width{0};
        int32_t height{0};

        uint16_t planes{1};
        uint16_t bit_count{0};
        uint32_t compression{0};
        uint32_t size_image{0};
        int32_t xppm{0};
        int32_t yppm{0};
        uint32_t colors_used{0};
        uint32_t important_colors{0};
    };
    struct ColorHeader {
        uint32_t red_mask{0x00ff0000};
        uint32_t green_mask{0x0000ff00};
        uint32_t blue_mask{0x000000ff};
        uint32_t alpha_mask{0xff000000};
        uint32_t color_space{0x73524742}; //sRGB
        uint32_t unused[16]{0}; //not used with sRGB
    };
    FileHeader fheader;
    InfoHeader iheader;
    ColorHeader cheader;
};
#endif