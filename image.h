#ifndef image_file
#define image_file
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
class Image {
    public:
    friend class BMP;
    friend class PNG;
    virtual void update_metadata() = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual void read(const std::string &filename) = 0;
    virtual void write(const std::string &filename) const = 0;
    void compress(int factor);
    void rotate(std::string direction);
    void flip(std::string axis);
    void enhance(int factor);
    void invert();
    void remove_color(std::string color);
    void contrast(int num);
    private:
    void flip_horizontal();
    void flip_vertical();
    void rotate_right();
    void rotate_left();
    void compress();
    std::string hex(int i) const;
    std::string convert_bytes(double bytes) const;
    int least_common_multiple(int num1, int num2) const;
    void clear_pixels();
    class Pixel {
        public:
        Pixel();
        Pixel(const Pixel &p);
        Pixel(int red, int green, int blue);
        void invert();
        void remove_color(std::string color);
        friend std::ostream& operator<<(std::ostream& os, Pixel& p) {
            os << std::setfill('0')  
            << std::setw(3) << int(p.r) << "," 
            << std::setw(3) <<  int(p.g) << "," 
            << std::setw(3) << int(p.b);
        }
        std::vector<int> get_rgb() const;
        void set_rgb(int red, int green, int blue);
        void contrast(int num);
        private:
        uint8_t r{0}, g{0}, b{0};

    };
    int width{0}, height{0};
    std::vector<Pixel*> *pixels;
};
#endif