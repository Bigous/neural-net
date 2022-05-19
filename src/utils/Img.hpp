#pragma once

#include "../math/Matrix2D.hpp"

struct Img {
    Matrix2D img_data;
    int label;

    void print();
};

std::vector<Img> csv_to_imgs(const std::string &file_string,
                             int number_of_imgs);
