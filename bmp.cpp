#include "bmp.h"
using namespace std;
BMP::BMP(const BMP &bmp) {
    fheader = bmp.fheader;
    iheader = bmp.iheader;
    cheader = bmp.cheader;
    for(int i = 0; i < bmp.pixels->size(); ++i) {
        Pixel *p = new Pixel(*bmp.pixels->at(i));
        pixels->push_back(p);
    }
    width = bmp.width;
    height = bmp.height;
}
BMP::BMP(const string &filename) {
    read(filename);
}
BMP::BMP(int32_t width, int32_t height) {
    iheader.width = uint8_t(width);
    iheader.height = uint8_t(height);
}
void BMP::update_metadata() {
    iheader.width = uint32_t(width);
    iheader.height = uint32_t(height);
    iheader.size_image = uint32_t(width*height*iheader.bit_count/8);
    fheader.file_size = uint32_t(iheader.size_image + fheader.offset_data);
}
void BMP::print(ostream &os) const {
    os << "BMP: " << image_name << endl;
    os << "File Header" << endl;
    os << "File Type: " << fheader.file_type << endl;
    if(fheader.file_type == 19778) {
        os << "\t (BMP)" << endl;
    } else {
        os << "\t (Unknown)" << endl;
    }
    os << "File Size: " << fheader.file_size << " bytes" << endl;
    os <<  "\t" << convert_bytes(fheader.file_size) << endl;
    os << "Reserved1: " << fheader.reserved1 << endl;
    os << "Reserved2: " << fheader.reserved2 << endl;
    os << "Offset Data: " << fheader.offset_data << " bytes" << endl;
    os << endl;
    os << "Info Header " << endl;
    os << "Size: " << iheader.size << " bytes" << endl;
    os << "Width: " << iheader.width << " pixels" << endl;
    os << "Height: " << iheader.height << " pixels" << endl;
    os << "Planes: " << iheader.planes << endl;
    os << "Bit Count: " << iheader.bit_count << endl;
    os << "Compression: " << iheader.compression << endl;
    os << "Image Size: " << iheader.size_image << " bytes " <<endl;
    os << "\t" << convert_bytes(iheader.size_image) << endl;
    os << "X Pixels Per Meter: " << iheader.xppm << endl;
    os << "Y Pixels Per Meter: " << iheader.yppm << endl;
    os << "Colors Used: " << iheader.colors_used << endl;
    os << "Important Colors: " << iheader.important_colors << endl;
    os << endl;
    if(iheader.bit_count == 32) {
        os << "Color Header" << endl;
        os << "Red Mask: " << hex(cheader.red_mask) << endl;
        os << "Green Mask: " << hex(cheader.green_mask) << endl;
        os << "Blue Mask: " << hex(cheader.blue_mask) << endl;
        os << "Alpha Mask: " << hex(cheader.alpha_mask) << endl;
        os << "Color Space: " << hex(cheader.color_space) << endl;
    }
}
void BMP::read(const string &filename) {
    set_image_name(filename);
    ifstream inp{filename, std::ios_base::binary};
    if(inp) {
        inp.read((char*)&fheader, sizeof(fheader));
        if(fheader.file_type != 0x4D42) {
            throw runtime_error("Not a BMP");
        }
        inp.read((char*)&iheader, sizeof(iheader));
        if(iheader.bit_count == 32) {
            inp.read((char*)&cheader, sizeof(cheader));
        }
        
        width = iheader.width;
        height = iheader.height;
        inp.seekg(fheader.offset_data, inp.beg);
        if(iheader.height < 0) {
            throw runtime_error("Origin can't be negative");
        }
        
        if(iheader.width % 4 == 0) {
            for(int i = 0; i < iheader.height*iheader.width; ++i) {
                uint8_t buffer[3];
                inp.read((char*)buffer, 3);
                Pixel *p = new Pixel(buffer[2], buffer[1], buffer[0]);
                pixels->push_back(p);
            }
        } else {
            uint8_t garb[3];
            for(int r = 0; r < iheader.height; ++r) {
                for(int c = 0; c < iheader.width; ++c) {
                    uint8_t buffer[3];
                    inp.read((char*)buffer, 3);
                    Pixel *p = new Pixel(buffer[2], buffer[1], buffer[0]);
                    pixels->push_back(p);
                }
                inp.read((char*)garb, iheader.width % 4);
            }

        }
        

    } else {
        throw runtime_error("Couldn't read file");
    }
    inp.close();
    update_metadata();
    return;
}
void BMP::write() const {
    ofstream out(image_name, ios_base::binary);
    out.write((char*)&fheader, sizeof(fheader));
    out.write((char*)&iheader, sizeof(iheader));
    if(iheader.bit_count == 32) {
        out.write((char*)&cheader, sizeof(cheader));
    }
    out.seekp(fheader.offset_data, out.beg);
    uint8_t buffer[3] = {0};
    for(int r = 0; r < iheader.height; ++r) {
        for(int c = 0; c < iheader.width; ++c) {
            buffer[0] = uint8_t(pixels->at(r*iheader.width + c)->get_rgb().at(2));
            buffer[1] = uint8_t(pixels->at(r*iheader.width + c)->get_rgb().at(1));
            buffer[2] = uint8_t(pixels->at(r*iheader.width + c)->get_rgb().at(0));
            
            out.write((char*)buffer, 3);
            
        }
        out.write((char*)buffer, iheader.width % 4);
    }
    out.close();
    return;
}
uint32_t BMP::make_stride_aligned(uint32_t align_stride) {
    uint32_t new_stride = row_stride;
    while(new_stride % align_stride != 0) {
        ++new_stride;
    }
    return new_stride;
}