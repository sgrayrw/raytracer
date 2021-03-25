// alinen, 2021
#include "ppm_image.h"
#include <cassert>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

using namespace agl;
using namespace std;
using namespace glm;

ppm_image::ppm_image() : myData(0), myWidth(0), myHeight(0) {
}

ppm_image::ppm_image(int width, int height) : myWidth(width), myHeight(height) {
    myData = new ppm_pixel[width * height];
}

ppm_image::ppm_image(const ppm_image &orig) {
    cout << "not implemented\n";
}

ppm_image &ppm_image::operator=(const ppm_image &orig) {
    if (&orig == this) {
        return *this;
    }

    cout << "not implemented\n";
    return *this;
}

ppm_image::~ppm_image() {
    delete[] myData;
}

bool ppm_image::save(const std::string &filename) const {
    int result = stbi_write_png(filename.c_str(), myWidth, myHeight,
                                3, (unsigned char *) myData, myWidth * 3);
    return (result == 1);
}

ppm_pixel ppm_image::get(int row, int col) const {
    assert(row >= 0 && row < myHeight);
    assert(col >= 0 && col < myWidth);
    return myData[row * myWidth + col];
}

void ppm_image::set(int row, int col, const ppm_pixel &color) {
    assert(row >= 0 && row < myHeight);
    assert(col >= 0 && col < myWidth);
    myData[row * myWidth + col] = color;
}

vec3 ppm_image::get_vec3(int i, int j) const {
    assert(i >= 0 && i < myHeight);
    assert(j >= 0 && j < myWidth);

    unsigned int idx = i * myWidth + j;
    ppm_pixel p = myData[idx];
    return glm::vec3(p.r / 255.0, p.g / 255.0, p.b / 255.0);
}

void ppm_image::set_vec3(int i, int j, const vec3 &c) {
    assert(i >= 0 && i < myHeight);
    assert(j >= 0 && j < myWidth);

    int idx = i * myWidth + j;
    myData[idx].r = (unsigned char) (c[0] * 255.999);
    myData[idx].g = (unsigned char) (c[1] * 255.999);
    myData[idx].b = (unsigned char) (c[2] * 255.999);
}
