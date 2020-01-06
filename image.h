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
    /*
    Default Image Constructor
    Allocates space on the heap for pointers to the pixels
    that are created later
    */
    Image();
    /*
    Default Image Destructor
    Clears all pixels from the heap and deletes the pixels pointer
    */
    ~Image();
    /*
    Updates the metadata of the image, different for each 
    type of image
    */
    virtual void update_metadata() = 0;
    /*
    Prints metadata of the image e.g. width, height, bit count, etc
    */
    virtual void print(std::ostream &os) const = 0;
    /*
    Reads image in from file and sets filename
    */
    virtual void read(const std::string &filename) = 0;
    /*
    Writes metadata and image data to binary file
    */
    virtual void write() const = 0;
    /*
    Sets image name, helps when printing metadata for image
    */
    void set_image_name(std::string name);
    /*
    Compresses the image by finding the smallest prime
    that is a factor of the width and height of the image,
    takes a square of pixels, creates a new pixel with the average
    RGB values from the square, and pushes it into a new data vector
    which replaces the original data vector. Repeated *factor* times
    */
    void compress(int factor);
    /*
    Rotates the image 90 degrees in the direction given,
    "right" or "left", calls rotate_right() and rotate_left()
    repsectively
    */
    void rotate(std::string direction);
    /*
    Gives the mirror image across the given axis,
    "horizontal" or "vertical" and calls flip_horizontal()
    and flip_vertical() respectively
    */
    void flip(std::string axis);
    /*
    Enlarges the image by taking one pixel and making it
    into four, height and width are doubled. Repeated *factor* times
    */
    void enhance(int factor);
    /*
    Inverts each pixel in the image by replacing subtracting each RGB
    value from 255
    */
    void invert();
    /*
    Sets the given color ("red", "green", "blue") to 0 
    for each pixel in the image
    */
    void remove_color(std::string color);
    /*
    Increases or decreases each RGB value by num
    if it is above or below 127 respectively
    */
    void contrast(int num);
    private:
    /*
    Flips the image horizontally by creating a new data
    vector, pushing a pointer to each pixel into the new vector,
    reordering the rows from 0 to height-1 to height-1 to 0
    */
    void flip_horizontal();
    /*
    Flips the image horizontally by creating a new data
    vector, pushing a pointer to each pixel into the new vector,
    reordering the columns from 0 to width-1 to width-1 to 0
    */
    void flip_vertical();
    /*
    Rotates the image to the right, reordering pointers to pixels
    in a new data vector and swapping width and height
    */
    void rotate_right();
    /*
    Rotates the image to the left, reordering pointers to pixels
    in a new data vector and swapping width and height
    */
    void rotate_left();
    /*
    Compresses image, replaces each square of pixels with the average 
    of the squares RGB values
    */
    void compress();
    /*
    Converts integer to hexadecimal equivalent string
    */
    std::string hex(int i) const;
    /*
    Converts bytes to least unit
    */
    std::string convert_bytes(double bytes) const;
    /*
    Returns the least common multiple of the two provided numbers
    */
    int least_common_multiple(int num1, int num2) const;
    /*
    Releases memory of each pixel in the image
    */
    void clear_pixels();
    class Pixel {
        public:
        /*
        Default Constructor for Pixel
        */
        Pixel();
        /*
        Copy Constructor for Pixel, copies pixel data to a 
        new pixel
        */
        Pixel(const Pixel &p);
        /*
        Constructs a new pixel with the given RGB value
        */
        Pixel(int red, int green, int blue);
        /*
        Inverts the pixel by subtracting the RGB values from 255
        */
        void invert();
        /*
        Sets the given color ("red", "green", "blue") to 0
        */
        void remove_color(std::string color);
        friend std::ostream& operator<<(std::ostream& os, Pixel& p) {
            os << std::setfill('0')  
            << std::setw(3) << int(p.r) << "," 
            << std::setw(3) <<  int(p.g) << "," 
            << std::setw(3) << int(p.b);
        }
        /*
        Returns the RGB values in a vector
        */
        std::vector<int> get_rgb() const;
        /*
        Sets the RGB of the pixel with the given values
        */
        void set_rgb(int red, int green, int blue);
        /*
        Raises or lowers each RGB value if it is higher or lower than 127
        */
        void contrast(int num);
        private:
        uint8_t r{0}, g{0}, b{0}; //holds rgb values between 0 and 255 inclusive

    };
    int width{0}, height{0}; //stores the width and height for temp use
                             //metadata should be updated whenever this is changed
    std::string image_name{"new_image"};
    std::vector<Pixel*> *pixels; //stores actual pixel data
};
#endif